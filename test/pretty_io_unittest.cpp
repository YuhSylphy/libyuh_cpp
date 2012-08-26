
#include <gtest/gtest.h>

#include "../include/yuh/adaptor/prettied.hpp"

#include <iostream>
#include <sstream>
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
using namespace yuh::adaptors;

using namespace boost::adaptors;

TEST(pretty_io, func_style)
{
	std::stringstream ss;

	//vector
	std::vector<int> v{1, 2, 3};
	ss << pretty(v);

	EXPECT_EQ(ss.str(), "{ 1, 2, 3 }");

	ss.str("");


	//array
	std::array<int, 4> a{{4, 3, 2, 1}};
	ss << pretty(a);
	
	EXPECT_EQ(ss.str(), "{ 4, 3, 2, 1 }");
	ss.str("");

	//reverse_range
	ss << pretty(a | reversed);
	EXPECT_EQ(ss.str(), "{ 1, 2, 3, 4 }");
	ss.str("");
}

TEST(pretty_io, pipe_operator)
{
	std::stringstream ss;

	//vector
	std::vector<int> v{1, 2, 3};
	ss << (v | prettied);

	EXPECT_EQ(ss.str(), "{ 1, 2, 3 }");

	ss.str("");


	//array
	std::array<int, 4> a{{4, 3, 2, 1}};
	ss << (a | prettied);
	
	EXPECT_EQ(ss.str(), "{ 4, 3, 2, 1 }");
	ss.str("");

	//reverse_range
	ss << (a | reversed | prettied);
	EXPECT_EQ(ss.str(), "{ 1, 2, 3, 4 }");
	ss.str("");
}
