#pragma once
#define BOOST_RESULT_OF_USE_DECLTYPE

#include <algorithm>
#include <numeric>
#include <random>

namespace yuh{
	//typedef std::vector<double>::iterator _InIt;
	//typedef lib::XORShift128 Random;


	/** ルーレット選択 */
	template<typename InIt, typename Random>
	InIt rouletteSel(
		InIt first,
		InIt last,
		Random& rnd)
	{
		typedef std::iterator_traits<InIt>::value_type value_type;

		/** 合計値 */
		const value_type max = 
			std::accumulate(
				first,
				last,
				value_type()
			);
		std::uniform_real<value_type> dst(0.0, max);
		/** 選択値 徐々に引いていく */
		auto temp = dst(rnd);

		auto ret = std::find_if(
			first,
			last,
			[&temp](const value_type& x) -> bool{
				if(temp <= x) return true;
				else {
					temp -= x;
					return false;
				}
			});

		return ret;
	}
	
	/** ルーレット選択 */
	template<typename Range, typename Random>
	inline auto rouletteSel(
		Range& _Seq,
		Random& rnd
		) //throw (std::range_error) 
		-> decltype(std::begin(_Seq))
	{
		//DOUT("ルーレット選択 Range\n");
		return rouletteSel(std::begin(_Seq), std::end(_Seq), rnd);
	}

}