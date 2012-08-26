
#include <gtest/gtest.h>

#include "../include/yuh/adaptor/set_powered.hpp"
#include "../include/yuh/adaptor/prettied.hpp"

#include <iostream>
#include <vector>
#include <array>
#include <iterator>
#include <boost/range/algorithm.hpp>
#include <boost/range/combine.hpp>
#include <set>
#include <boost/format.hpp>
#include <boost/range/adaptor/reversed.hpp>


using std::cout;
using std::endl;

using namespace yuh;

using namespace boost::adaptors;


TEST(set_power, direct)
{

	std::vector<int> s{ 1, 2, 3 };
	std::vector<std::vector<int> > ans {
		{}, {1}, {2}, {1, 2}, {3}, {1, 3}, {2, 3}, {1, 2, 3}
	};
	
	range_detail::set_powered_range<std::vector<int> >
		rng(s);
	
	for ( auto t: boost::combine(rng, ans) )
	{
		EXPECT_TRUE(boost::equal(boost::get<0>(t), boost::get<1>(t)));
	}
	
	for ( auto t: boost::combine(rng | reversed, ans | reversed) )
	{
		EXPECT_TRUE(boost::equal(boost::get<0>(t), boost::get<1>(t)));
	}

}

TEST(set_power, pipe)
{
	using namespace yuh::adaptors;
	
	std::vector<int> s{ 1, 2, 3 };
	std::vector<std::vector<int> > ans {
		{}, {1}, {2}, {1, 2}, {3}, {1, 3}, {2, 3}, {1, 2, 3}
	};
	
	for ( auto t: boost::combine(s | set_powered, ans) )
	{
        EXPECT_TRUE(boost::equal(boost::get<0>(t), boost::get<1>(t)));
	}
	
	for ( auto t: boost::combine(s | set_powered | reversed, ans | reversed) )
	{
        EXPECT_TRUE(boost::equal(boost::get<0>(t), boost::get<1>(t)));
	}
	
}

TEST(set_power, func)
{
	std::vector<int> s{ 1, 2, 3 };
	std::vector<std::vector<int> > ans {
		{}, {1}, {2}, {1, 2}, {3}, {1, 3}, {2, 3}, {1, 2, 3}
	};
	
	for ( auto t: boost::combine(adaptors::set_power(s), ans) )
	{
        EXPECT_TRUE(boost::equal(boost::get<0>(t), boost::get<1>(t)));
	}
	
	for ( auto t: boost::combine(adaptors::set_power(s) | reversed, ans | reversed) )
	{
        EXPECT_TRUE(boost::equal(boost::get<0>(t), boost::get<1>(t)));
	}
	
}
