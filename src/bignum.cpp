#include "stdafx.h"
#include "../include/yuh/bignum.h"

#include <boost/range/adaptor/reversed.hpp>
#include <boost/range/irange.hpp>

#include "../include/yuh/adaptor/prettied.hpp"


using namespace boost::adaptors;
using namespace yuh::adaptors;
namespace yuh
{
	std::string bignum::hex() const
	{
		std::string ret((sign_?1:0) + 8*d_.size(), '\0');
		
		auto it = std::begin(ret);

		if(sign_)
		{
			*it = '-';
			++it;
		}

		for( auto x: d_ | reversed )
		{
			boost::copy(
				(boost::format("%|08X|") % x).str(),
				it
				);
			std::advance(it, 8);
		}

		return ret;
	}

	bool bignum::operator<(const bignum& rhs) const
	{
		if(sign_!=rhs.sign_) return sign_; //符号違い，こちらがマイナスでtrue
		bool ret; // 以下とりあえず正として
		if(d_.size()!=rhs.d_.size()) ret = d_.size() < rhs.d_.size(); //長いほうがでっかい
		else
		{
			int i = d_.size() - 1;
			for( ; i >= 0; --i )
			{
				if(d_[i] != rhs.d_[i])
				{
					ret = d_[i] < rhs.d_[i];
					break;
				}
			} 
			if (i < 0) return false; // 一致
		}
		return sign_ ? !ret : ret; //負だったら逆
	}
		
	bignum& bignum::operator<<=(int rhs)
	{
		auto x = rhs;
		// std::cout << rhs << (d_ | prettied("0x%|08X|", "[", " ", "]" )) << std::endl;
		for (; x >= 32; x-=32) d_.push_front(0U);

		digit_t mask = ((1U << x)-1) << ( 32 - x ); //上位rhsビット
			
		if( 0 <  (d_.back() & mask) )
		{   //繰り上がりあり
			d_.push_back(0U);
		}
			
		auto prev = d_.rbegin();
		auto it = std::next(prev);
			
		*prev <<= rhs;
		for ( ; it!=d_.rend(); ++it )
		{
			*prev |= (*it & mask) >> (32 - rhs);
			*it <<= rhs;
			prev = it;
		}
		
		return *this;
	}

	bignum& bignum::operator>>=(int rhs)
	{
		auto x = rhs;
		for (; x >= 32; x-=32) d_.pop_front();

		digit_t mask = (1U << x)-1; //下位rhsビット
			
		auto prev = d_.begin();
		auto it = std::next(prev);
			
		*prev >>= rhs;
		for ( ; it!=d_.end(); ++it )
		{
			*prev |= (*it & mask) << (32 - rhs);
			*it >>= rhs;
			prev = it;
		}
		// std::cout << (d_ | prettied("%|08X|")) << std::endl;
		if ( 0 == d_.back() && d_.size() > 1U )
		{
			d_.pop_back();
		}
		
		return *this;
	}

	bignum& bignum::operator+=(const bignum& rhs)
	{
		if( sign_ != rhs.sign_ ) 
			return *this-=(-rhs); 
		
		twice_t C = 0;
		twice_t mask = (1LL << 32) - 1;

		if ( rhs.d_.size() > d_.size() )
		{
			d_.resize(rhs.d_.size(), 0U);
		}

		uint32_t i = 0;
		for( ; i < rhs.d_.size(); ++i)
		{
			const twice_t t = C + d_[i] + rhs.d_[i];
			d_[i] = mask & t;
			C = t >> 32;
		}
		
		for( ; i < d_.size() && C > 0; ++i )
		{
			const twice_t t = C + d_[i];
			d_[i] = mask & t;
			C = t >> 32;
		}

		if(C>0)
		{
			d_.push_back(static_cast<digit_t>(C));
		}
		
		return *this;
	}
	
	bignum& bignum::operator-=(const bignum& rhs) 
	{
		if ( sign_ != rhs.sign_ )
			return *this+=(-rhs);
		if ( sign_ ? (*this > rhs) : (*this < rhs) )
		{ // 絶対値が引く数のが大きい
			*this = (-rhs) - (-(*this));
			return *this;
		}
		
		digit_t B = 0;
		twice_t mask = (1LL << 32) - 1;

		if ( rhs.d_.size() > d_.size() )
		{   //サイズ違い
			d_.resize(rhs.d_.size(), 0U);
		}

		uint32_t i = 0;
		for( ; i < rhs.d_.size(); ++i )
		{
			const twice_t t = (1LL << 32) + d_[i] - rhs.d_[i] - B;
			d_[i] = mask & t;
			B = 1 - (t >> 32);
		}
		for( ; i < d_.size() && B > 0; ++i )
		{
			const twice_t t = (1LL << 32) + d_[i] - B;
			d_[i] = mask & t;
			B = 1 - (t >> 32);

		}
		if( B > 0 )
		{   //チェックしてるから無いはずだけど．
			sign_ = !sign_;
		}

		// 上のほうが0なのではずす
		while( d_.back() == 0U && d_.size() > 1U ) d_.pop_back();

		return *this;
	}

	/**
	 * 除算.
	 * わる数は一桁分だけ
	 * @param rhs 右辺
	 * @return 商
	 */
	bignum& bignum::operator/=(digit_t rhs)
	{
		std::deque<digit_t> deq;
		twice_t d; // 割られる数
		for( auto x: d_ | reversed ) // 上位から
		{
			d <<= 32; // 前回の余りをシフトしておいて
			d += x;   // 次の桁を足すと今回の割られる数
			deq.push_front(d / rhs); // 商を追加
			d %= rhs; // 余りが次の割られる数に
		}
		std::swap(d_, deq);

		// 上のほうが0なのではずす
		while( d_.back() == 0U && d_.size() > 1U ) d_.pop_back();
		
		// std::cout << "div " << this->hex() << std::endl;
		return *this;
	}
	
	bignum::bignum(const std::string& str)
		: sign_(false), d_(1)
	{
		auto it = std::begin(str);

		switch(*it)
		{
		case '-':
			sign_ = true;
		case '+':
			++it;
		default:
			break;
		}

		std::for_each(
			it, std::end(str),
			[this](char c)
			{
				auto digit = static_cast<digit_t>(c) - '0';
				
				auto tmp = *this << 3; //*8
				*this<<=1;             //*2
				*this+=tmp;            //*10
				
				d_[0]+=digit;
			});
	}

	/**
	 * 十進文字列化
	 * @return 十進文字列
	 */
	std::string bignum::to_string() const
	{
		static const auto log2 = 0.301029996;

		int i = 31;
		for(; (d_.back() & ( 1<<i )) == 0; --i );
		const auto width =  //幅最大値，のはず
			1 + //floor->ceil
			static_cast<digit_t>(
				log2 * 
				(32 * (d_.size() - 1) + i + 1 ) //桁数
				) 
			+ (sign_?1:0);
		
		std::string ret(width, '\0');
		
		auto it = ret.rbegin();
		auto tmp = *this;
		static const bignum zero(0);
		if(sign_) 
		{
			ret[0] = '-';
			tmp.sign_=false;
		}

		while(tmp != zero)
		{
			auto q = bignum(tmp) /= 10;
			auto r = (tmp - ((q<<3)+(q<<1))).d_[0]; 
			*it = static_cast<char>('0' + r);
			tmp.swap(q);
			++it;
		}

		return ret;
	}

	bignum operator"" _B(const char* str)
	{
		return bignum(std::string(str));
	}
	
	void bignum::swap(bignum& rhs) throw()
	{
		std::swap(sign_, rhs.sign_);
		std::swap(d_, rhs.d_);
	}

	void swap(bignum&lhs, bignum&rhs) throw()
	{ lhs.swap(rhs); }

}

namespace std
{
	void swap(yuh::bignum&lhs, yuh::bignum&rhs) throw()
	{ lhs.swap(rhs); }
}
