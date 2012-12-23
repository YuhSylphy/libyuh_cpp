#include <gtest/gtest.h>

#include "../include/yuh/cartesian.hpp"

#include <boost/format.hpp>
#include <boost/range/combine.hpp>

#include <vector>

#include <cxxabi.h>


using namespace yuh;
namespace std{using namespace tr1;}

TEST(cartesian, comp)
{
	std::vector<int> x{ 1, 2, 3 };
	std::vector<char> y{ 'a', 'b', 'c' };
	std::vector<std::tuple<int, char> > ans =
	{
		std::make_tuple(1, 'a'),
		std::make_tuple(2, 'a'),
		std::make_tuple(3, 'a'),
		std::make_tuple(1, 'b'),
		std::make_tuple(2, 'b'),
		std::make_tuple(3, 'b'),
		std::make_tuple(1, 'c'),
		std::make_tuple(2, 'c'),
		std::make_tuple(3, 'c'),
	};

	for(auto t: boost::combine(cartesian(x, y), ans) )
	{
		EXPECT_EQ(boost::get<0>(t), boost::get<1>(t));
	}
	// std::vector<std::string> z{"prev", "next"};
	// for(auto t: cartesian(x, y, z) )
	// {
	// 	std::cout << 
	// 		(boost::format("(%||, %||, %||), ") 
	// 		 % std::get<0>(t) 
	// 		 % std::get<1>(t)
	// 		 % std::get<2>(t) );
	// }
	// std::cout << std::endl;
	
}

