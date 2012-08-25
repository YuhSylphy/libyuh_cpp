/**
 * クラスにする程でもない数学系の関数詰め合わせ
 * @author Yuh Sylphy
 */

#pragma once

#include <cmath>

#include <boost/assert.hpp>
#include <boost/range/distance.hpp>
#include <stdexcept>


namespace yuh
{
	/**
	 * 回文数ならTrue
	 * @param n 非負整数
	 * @return nが回文数かどうか
	 */
	template<typename Integer>
	bool is_palindromic(Integer n)
	{
        Integer c = 0;
        Integer t = n;
        const Integer base = 10;

		// 10倍して桁を移す->下一桁を移す
        while(t > 0)
        {
			const auto digit = t % base;
			c *= base;
			c += digit;
			t = (t - digit) / base;
        }
		// 反転しても等しければ回文数
        return c==n;
	}

	/**
	 * ピタゴラス数であるかを判定
	 * 範囲チェック省略
	 * @param Integer 整数型
	 * @param a 隣辺
	 * @param b 隣辺
	 * @param c 斜辺
	 * @return 三つ組(a,b,c)がピタゴラス数であるか
	 */
	template<typename Integer>
	inline bool is_Pythagorean(Integer a, Integer b, Integer c)
	{
		// 途中で関数にするまでもないと思ったけど
		// 意味がはっきりとはしそうなので最後まで書く
		return a*a + b*b == c*c;
	}

	/**
	 * ピタゴラス数であるかを判定
	 * @param Range SinglePathRange
	 * @param rng a,b,cを格納したSinglePathRange
	 * @return 三つ組(a,b,c)がピタゴラス数であるか
	 */
	template<typename Range>
	inline bool is_Pythagorean(const Range& rng)
	{
		if(boost::distance(rng) != 3) 
			throw std::invalid_argument("is_Pythagorean: range size error");

		auto it = std::begin(rng);
		const auto& a = *it;
		const auto& b = *(++it);
		const auto& c = *(++it);

		return a*a + b*b == c*c;
	}
	/**
	 * 平方数判定
	 * @param Integer 整数型
	 * @param n チェックする値
	 * @return 平方数であればtrue
	 */
	template<typename Integer>
	bool is_square(Integer n)
	{
		return 	n == std::pow(std::floor(std::sqrt(n)), 2);
	}
}
