#include "../include/yuh/prime.h"

#include <vector>
#include <list>

#include <iostream>

namespace yuh
{
	namespace range_detail
	{
		std::vector<prime_iterator::value_type> prime_iterator::p_{ 0, 2, 3, 5, 7, 11 };

		void prime_iterator::sift(value_type upper)
		{
			std::list<value_type> candidate{};

			{   //初期化
				auto c = p_.back();

				auto s = 0;
			
				if(c % 6 == 1) // 2,3を覗いたすべての素数は6で割るとあまり1か5
					s = 4;
				else
					s = 2;
			
				auto step = [&]{
					c += s;           //3の倍数を除いた奇数を順にたどる
					s = s==2 ? 4 : 2; //ステップをスイッチ
				};
				step();
			
				for(; c < upper; step())
				{ //候補を追加していく
					candidate.push_back(c);
				}
			}

			for( auto i = 3U; i < p_.size(); i++ )
			{   //5から走査開始
				const auto p = p_[i];
				for ( auto it = std::begin(candidate);
					  it != std::end(candidate);)
				{
					if(p * p > *it)
					{   //これ以上は探しても割り切れないことがわかっているので素数リストに追加
						p_.push_back(*it);
						candidate.erase(it++);
					}
					else if(*it % p == 0)
					{   //割りきれてしまったので候補から除外
						candidate.erase(it++);
					}
					else
					{
						++it;
					}
				}
			}
			
			BOOST_ASSERT( candidate.size() == 0 );
		}
		
		prime_iterator::value_type prime_iterator::prime_approx(size_t n)
		{
			const auto a = std::log10(n);
			const auto b = 1.4/(a+0.49)+1.96*std::log10(a+0.49)+1.13675*(a+0.49)-2.47;
			return std::ceil(2*(b*n+n)-1);
			/*
			  177 ：NAS6 ◆n3AmnVhjwc ：2012/05/10(木) 21:48:11.71
			  n番目の素数Pnの概算 
			  a=logn 
			  b=1.4/(a+0.49)+1.96log(a+0.49)+1.13675(a+0.49)-2.47 
			  Pn=2*(b*n+n)-1 
			  実際P 
			  a=1,Pn=29,P=29 
			  a=2,Pn=538,P=541 
			  a=3,Pn=7923,P=7919 
			  a=4,Pn=104483,P=104729 
			  a=17,Pn=4185534253400260096,P=4185296581467695669 
			*/
		}

		const decltype(prime_iterator::p_)& prime_iterator::get_prime()
		{
			return p_;
		}

		void prime_iterator::expand(int index)
		{
			//概算よりちょっとだけ大きいところまで計算 無駄も出るけど
			auto approx = prime_approx(index);
			
			while( index >= static_cast<int>(p_.size()) )
			{
				sift(approx*=1.05);
			}
		}

		prime_iterator::prime_iterator(int index)
			:index_(index)
		{
			expand(index);
		}


		void prime_iterator::increment()
		{
			index_++;
			if( index_ <= p_.size() ) 
				expand(index_); //まだ知らない素数だったら調べて追加する
			BOOST_ASSERT( index_ < static_cast<int>(p_.size()) );
		}
		
		void prime_iterator::decrement()
		{
			if(index_ >= 0)
				index_--;
		}
		
		bool prime_iterator::equal(const prime_iterator& rhs) const
		{
			return rhs.index_ == index_; 
		}
		
		prime_iterator::reference prime_iterator::dereference() const
		{
			BOOST_ASSERT( index_ < static_cast<int>(p_.size()) );
			return p_[index_];
		}
		
		void prime_iterator::advance(difference_type offset)
		{
			index_ += offset;
			if(index_ < static_cast<int>(p_.size()) ) expand(index_); //まだ知らない素数だったら調べて追加する
			BOOST_ASSERT( index_ < static_cast<int>(p_.size()) );
		}
		
		prime_iterator::difference_type prime_iterator::distance_to(const prime_iterator& rhs) const
		{
			return rhs.index_ - index_;
		}
		
	} // namespace range_detail
	
	
	prime_range prime(int first)
	{
		BOOST_ASSERT( first >= 1 );
		return prime_range(first);
	}
	
	prime_range prime(int first, int last)
	{
		BOOST_ASSERT( first <= last );
		BOOST_ASSERT( first >= 1 );
		return prime_range(first, last);
	}
	
} // namespace yuh

