
#include <gtest/gtest.h>

#include "../include/yuh/adaptors/count_mapped.hpp"

#include <vector>
#include <array>
#include <iterator>
#include <boost/range/algorithm.hpp>
#include <boost/range/combine.hpp>
#include <set>
#include <boost/format.hpp>
#include <boost/range/adaptor/reversed.hpp>


using namespace yuh;

using namespace boost::adaptors;

namespace yuh 
{
	using std::ostream;
	template<typename T, typename U>
	ostream&operator<<(ostream& os, std::pair<T, U> p)
	{
		return os << boost::format("{ %||, %|| }") % p.first % p.second;
	} 
}


TEST(count_map, direct)
{
	
	std::multiset<int> s{ 2, 2, 2, 3, 3, 5 };
	std::vector<std::pair<const int, int> > ans {
		{2, 3}, {3, 2}, {5, 1}
	};
	
	
	range_detail::count_mapped_range<std::multiset<int> >
		rng(s);
	
	for ( auto p: boost::combine(rng, ans) )
	{
		EXPECT_EQ(boost::get<0>(p).first, boost::get<1>(p).first);
		EXPECT_EQ(boost::get<0>(p).second, boost::get<1>(p).second);
	}
	
	for ( auto p: boost::combine(rng | reversed, ans | reversed) )
	{
		EXPECT_EQ(boost::get<0>(p).first, boost::get<1>(p).first);
		EXPECT_EQ(boost::get<0>(p).second, boost::get<1>(p).second);
	}

}

TEST(count_map, pipe)
{
	using namespace yuh::adaptor;
	
	std::multiset<int> s{ 2, 2, 2, 3, 3, 5, 7, 7};
	std::vector<std::pair<const int, int> > ans {
		{2, 3}, {3, 2}, {5, 1}, {7, 2}
	};
	
	for ( auto p: boost::combine(s | count_mapped, ans) )
	{
		EXPECT_EQ(boost::get<0>(p).first, boost::get<1>(p).first);
		EXPECT_EQ(boost::get<0>(p).second, boost::get<1>(p).second);
	}
	
	for ( auto p: boost::combine(s | count_mapped | reversed, ans | reversed) )
	{
		EXPECT_EQ(boost::get<0>(p).first, boost::get<1>(p).first);
		EXPECT_EQ(boost::get<0>(p).second, boost::get<1>(p).second);
	}
	
}

TEST(count_map, func)
{
	std::multiset<int> s{ 2, 2, 2, 3, 3, 5 };
	std::vector<std::pair<const int, int> > ans {
		{2, 3}, {3, 2}, {5, 1}
	};
	
	for ( auto p: boost::combine(adaptor::count_map(s), ans) )
	{
		EXPECT_EQ(boost::get<0>(p).first, boost::get<1>(p).first);
		EXPECT_EQ(boost::get<0>(p).second, boost::get<1>(p).second);
	}
	
	for ( auto p: boost::combine(adaptor::count_map(s) | reversed, ans | reversed) )
	{
		EXPECT_EQ(boost::get<0>(p).first, boost::get<1>(p).first);
		EXPECT_EQ(boost::get<0>(p).second, boost::get<1>(p).second);
	}
	
}
