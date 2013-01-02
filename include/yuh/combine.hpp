#pragma once

#include <boost/range/combine.hpp>
#include <boost/range/adaptor/taken.hpp>

namespace yuh
{
	namespace detail
	{
		template<typename Range1, typename Range2>
		auto combine_delegater(Range1& rng1, Range2& rng2)
			-> boost::range_detail::zip_range<
			boost::range_detail::taken_range<Range1>,
			boost::range_detail::taken_range<Range2> >
		{
			auto length = std::min(
				boost::distance(rng1), 
				boost::distance(rng2));
			auto t1 = rng1 | boost::adaptors::taken(length);
			auto t2 = rng2 | boost::adaptors::taken(length);
			return boost::combine(t1, t2);
		}
	}

	template<typename Range1, typename Range2>
	auto combine(Range1& rng1, Range2& rng2) 
		-> decltype(detail::combine_delegater<Range1, Range2>(rng1, rng2))
	{
		return detail::combine_delegater<Range1, Range2>(rng1, rng2);
	}
	template<typename Range1, typename Range2>
	auto combine(Range1 const& rng1, Range2& rng2) 
		-> decltype(detail::combine_delegater<Range1 const, Range2>(rng1, rng2))
	{
		return detail::combine_delegater<Range1 const, Range2>(rng1, rng2);
	}
	template<typename Range1, typename Range2>
	auto combine(Range1& rng1, Range2 const& rng2) 
		-> decltype(detail::combine_delegater<Range1, Range2 const>(rng1, rng2))
	{
		return detail::combine_delegater<Range1, Range2 const>(rng1, rng2);
	}
	template<typename Range1, typename Range2>
	auto combine(Range1 const& rng1, Range2 const& rng2) 
		-> decltype(detail::combine_delegater<Range1 const, Range2 const>(rng1, rng2))
	{
		return detail::combine_delegater<Range1 const, Range2 const>(rng1, rng2);
	}
}