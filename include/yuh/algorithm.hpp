#include <algorithm>
#include <boost/range.hpp>

namespace boost
{
	template<typename RandomAccessRange, 
			 typename UniformRandomNumberGenerator>
	inline RandomAccessRange& shuffle(
		RandomAccessRange& rng,
		UniformRandomNumberGenerator && g
		)
	{
		BOOST_RANGE_CONCEPT_ASSERT(( RandomAccessRangeConcept<RandomAccessRange> ));
		::std::shuffle(boost::begin(rng), boost::end(rng), g);
		return rng;
	}

	template<typename RandomAccessRange, 
			 typename UniformRandomNumberGenerator>
	inline RandomAccessRange const& shuffle(
		RandomAccessRange const& rng,
		UniformRandomNumberGenerator && g
		)
	{
		BOOST_RANGE_CONCEPT_ASSERT(( RandomAccessRangeConcept<RandomAccessRange const> ));
		::std::shuffle(boost::begin(rng), boost::end(rng), g);
		return rng;
	}
}

