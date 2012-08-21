#pragma once

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
}
