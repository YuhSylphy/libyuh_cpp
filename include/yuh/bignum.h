#pragma once
#include <type_traits>
#include <deque>
#include <string>
#include <iostream>
#include <utility>
#include <boost/format.hpp>
#include <boost/range/algorithm.hpp>

#include "detail/enabler.h"

namespace yuh
{
	class bignum
	{
	public:
		typedef uint32_t digit_t;
		typedef uint64_t twice_t;

		/*
		 * ctor
		 * @param Integer 整数型 32bitを超えるものは未対応 多分切り捨てられる
		 * @param n 変換元
		 */
		template<typename Integer, 
				 typename std::enable_if<std::is_integral<Integer>::value>::type *& = enabler>
		bignum(Integer n)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wtautological-compare"
			: sign_( n < 0 ), d_(1)
#pragma clang diagnostic pop
		{
			d_[0] = sign_? -n: n;
		}
		
		/*
		 * ctor
		 * 内部表現をそのまま渡す
		 * @param sign 符号
		 * @param r 32bit毎の符号なし整数列
		 */
		template<typename Range, 
				 typename = typename boost::range_value<Range>::type>
		bignum(bool sign, const Range& r)
		: sign_(sign), d_(0)
		{
			boost::copy(r, std::back_inserter(d_));
		}

		/**
		 * from string
		 * リテラルの対応がめんどくさいのでwstringやらは必要になったらオーバーロードする
		 * @param str 整数を表す文字列
		 */
		bignum(const std::string& str);

		/**
		 * copy ctor
		 */
		bignum(const bignum& rhs)
			: sign_(rhs.sign_), d_(rhs.d_) {}
		
		/**
		 * 十六進文字列化
		 * @return 十六進文字列
		 */
		std::string hex() const;
		
		/**
		 * 十進文字列化
		 * @return 十進文字列
		 */
		std::string to_string() const;
		
		/**
		 * 左シフト代入
		 * @param rhs シフト桁数
		 * @return 自身
		 */
		bignum& operator<<=(int rhs);
		/**
		 * 左シフト
		 * @param rhs シフト桁数
		 * @return 結果rvalue
		 */
		inline bignum operator<<(int rhs) const
		{ return bignum(*this)<<=rhs; }
		/**
		 * 右シフト代入
		 * @param rhs シフト桁数
		 * @return 自身
		 */
		bignum& operator>>=(int rhs);
		/**
		 * 右シフト
		 * @param rhs シフト桁数
		 * @return 結果rvalue
		 */
		inline bignum operator>>(int rhs) const
		{ return bignum(*this)>>=rhs; }
		/**
		 * 単項マイナス
		 */
		inline bignum operator-() const
		{ 
			bignum ret(*this);
			ret.sign_ = !sign_;
			return ret;
		}
		/**
		 * 単項プラス
		 */
		inline bignum operator+() const
		{ 
			return *this;
		}
		/**
		 * 加算代入
		 * @param rhs 右辺
		 * @return 自身
		 */
		bignum& operator+=(const bignum& rhs);
		/**
		 * 加算代入
		 * @param rhs 右辺
		 * @return 自身
		 */
		inline bignum& operator+=(bignum&& rhs)
		{ return *this+=rhs; }
		/**
		 * 加算代入
		 * @param rhs 右辺
		 * @return 自身
		 */
		template <typename T>
		inline bignum operator+(const T& rhs) const
		{ return bignum(*this)+=rhs; }
		/**
		 * 加算代入
		 * @param rhs 右辺
		 * @return 自身
		 */
		template <typename T>
		inline bignum operator+(T&& rhs) const
		{ return bignum(*this)+=rhs; }
		/**
		 * 減算代入
		 * @param rhs 右辺
		 * @return 自身
		 */
		bignum& operator-=(const bignum& rhs);
		/**
		 * 減算代入
		 * @param rhs 右辺
		 * @return 自身
		 */
		bignum& operator-=(bignum&& rhs)
		{ return *this-=rhs; }
		/**
		 * 減算
		 * @param rhs 右辺
		 * @return 結果rvalue
		 */
		template <typename T>
		inline bignum operator-(const T& rhs) const
		{ return bignum(*this)-=rhs; }
		/**
		 * 減算
		 * @param rhs 右辺
		 * @return 結果rvalue
		 */
		template <typename T>
		inline bignum operator-(T&& rhs) const
		{ return bignum(*this)-=rhs; }
		/**
		 * 除算.
		 * わる数は一桁分だけ
		 * @param rhs 右辺
		 * @return 商
		 */
		bignum& operator/=(digit_t rhs);
		/**
		 * 非等値
		 * @param rhs 右辺
		 * return *this != rhs
		 */
		bool operator!=(const bignum& rhs) const
		{ return sign_!=rhs.sign_ || d_!=rhs.d_; }
		
		/**
		 * 非等値
		 * @param rhs 右辺
		 * return *this == rhs
		 */
		template<typename T>
		bool operator==(const T& rhs) const
		{ return !(*this!=rhs); }
		
		/**
		 * operator<
		 * @param rhs 右辺
		 * return *this < rhs
		 */
		bool operator<(const bignum& rhs) const;
		
		/**
		 * operator>
		 * @param rhs 右辺
		 * return *this > rhs
		 */
		template<typename T>
		inline bool operator>(const T& rhs) const
		{ return (rhs < *this); } 
		/**
		 * operator<=
		 * @param rhs 右辺
		 * return *this <= rhs
		 */
		template<typename T>
		inline bool operator<=(const T& rhs) const
		{ return !(rhs < *this); }
		/**
		 * operator>=
		 * @param rhs 右辺
		 * return *this >= rhs
		 */
		template<typename T>
		inline bool operator>=(const T& rhs) const
		{ return !(*this < rhs); }

		/**
		 * 絶対値
		 * @return 符号をfalseにしたもの
		 */
		inline bignum abs() const
		{
			bignum ret(*this);
			ret.sign_ = false;
			return ret;
		}
		/**
		 * 出力
		 * @param os 出力先ストリーム
		 * @return osそのもの
		 */
		inline std::ostream&output(std::ostream&os) const
		{
			return os << this->to_string();
		}
		/**
		 * swap
		 * @param rhs 右辺
		 */
		void swap(bignum& rhs) throw();
	private:
		/**
		 * 符号
		 */
		bool sign_;
		/**
		 * 32bit毎内部表現
		 */
		std::deque<digit_t> d_;
	};
	/**
	 * global swap overload
	 * @param lhs 左辺
	 * @param rhs 右辺
	 */
	void swap(bignum&lhs, bignum&rhs) throw();
	
	/**
	 * ユーザ定義リテラル_B
	 */
	bignum operator"" _B(const char*);

	/**
	 * 出力ストリーム
	 * to_stringの結果を出力
	 * @param os 出力先
	 * @param rhs 出力するbignum
	 * @return os
	 */
	inline std::ostream& operator<<(std::ostream& os, const bignum& rhs)
	{
		return rhs.output(os);
	}
}

namespace std
{
	/**
	 * global swap overload
	 * @param lhs 左辺
	 * @param rhs 右辺
	 */
	void swap(yuh::bignum&lhs, yuh::bignum&rhs) throw();
}

