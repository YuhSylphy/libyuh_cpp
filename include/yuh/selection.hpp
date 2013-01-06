#pragma once
#define BOOST_RESULT_OF_USE_DECLTYPE

#include <algorithm>
#include <numeric>
#include <random>

namespace yuh{
	//typedef std::vector<double>::iterator _InIt;
	//typedef lib::XORShift128 Random;


	/** ���[���b�g�I�� */
	template<typename InIt, typename Random>
	InIt rouletteSel(
		InIt first,
		InIt last,
		Random& rnd)
	{
		typedef std::iterator_traits<InIt>::value_type value_type;

		/** ���v�l */
		const value_type max = 
			std::accumulate(
				first,
				last,
				value_type()
			);
		std::uniform_real<value_type> dst(0.0, max);
		/** �I��l ���X�Ɉ����Ă��� */
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
	
	/** ���[���b�g�I�� */
	template<typename Range, typename Random>
	inline auto rouletteSel(
		Range& _Seq,
		Random& rnd
		) //throw (std::range_error) 
		-> decltype(std::begin(_Seq))
	{
		//DOUT("���[���b�g�I�� Range\n");
		return rouletteSel(std::begin(_Seq), std::end(_Seq), rnd);
	}

}