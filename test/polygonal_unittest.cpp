#include <gtest/gtest.h>

#include "../include/yuh/polygonal.hpp"

#include <vector>
#include <array>
#include <iterator>
#include <boost/range/algorithm.hpp>
#include <boost/range/combine.hpp>

using namespace yuh;

TEST(trigonal_iterator, ctor_raw_values) 
{
	range_detail::polygonal_iterator<3, int> it(1);

	EXPECT_EQ(*it++, 1);
	EXPECT_EQ(*it++, 3);
	EXPECT_EQ(*it++, 6);
	EXPECT_EQ(*it++, 10);
	EXPECT_EQ(*it++, 15);
	EXPECT_EQ(*it++, 21);
	EXPECT_EQ(*it++, 28);
	EXPECT_EQ(*it++, 36);
	EXPECT_EQ(*it++, 45);
	EXPECT_EQ(*it++, 55);
	EXPECT_EQ(*it++, 66);

	EXPECT_EQ(*it--, 78);
	EXPECT_EQ(*it--, 66);
	EXPECT_EQ(*it--, 55);
	EXPECT_EQ(*it--, 45);
	EXPECT_EQ(*it--, 36);
	EXPECT_EQ(*it--, 28);
	EXPECT_EQ(*it--, 21);
	EXPECT_EQ(*it--, 15);
	EXPECT_EQ(*it--, 10);
	EXPECT_EQ(*it--, 6);
	EXPECT_EQ(*it--, 3);
	EXPECT_EQ(*it--, 1);

}

TEST(trigonal_iterator, ctor_index)
{
	typedef range_detail::polygonal_iterator<3, int> iter_type;
	EXPECT_EQ(*iter_type(1), 1);
	EXPECT_EQ(*iter_type(2), 3);
	EXPECT_EQ(*iter_type(3), 6);
	EXPECT_EQ(*iter_type(4), 10);
	EXPECT_EQ(*iter_type(5), 15);
	EXPECT_EQ(*iter_type(6), 21);
	EXPECT_EQ(*iter_type(7), 28);
	EXPECT_EQ(*iter_type(8), 36);
	EXPECT_EQ(*iter_type(9), 45);
	EXPECT_EQ(*iter_type(10), 55);
}

TEST(trigonal_iterator, distance)
{
	typedef range_detail::polygonal_iterator<3, int> iter_type;
	
	EXPECT_EQ(std::distance(iter_type(1), iter_type(4)), 3);
	EXPECT_EQ(std::distance(iter_type(5), iter_type(5)), 0);
	// EXPECT_EQ(std::distance(iter_type(3), iter_type(2)), -1); //無限ループ
}

TEST(trigonal_range, ctor_finite)
{
	const auto check = [](
		const polygonal_range<3, int>& tgt,
		const std::vector<int>& exp
		) 
	{
	boost::for_each(
		boost::combine(tgt, exp),
		[](const boost::tuple<int , int>& t) {
			EXPECT_EQ(boost::get<0>(t), boost::get<1>(t));
		}
		);
	};

	check(polygonal_range<3, int>(0, 10),
		  {0,1,3,6,10,15,21,28,36,45}
		);
	check(polygonal_range<3, int>(10, 15),
		  {55,66,78,91,105}
		);
}

TEST(trigonal_range, ctor_infinite)
{
	int i = 1;
	static const std::array<int, 25> a {{ 
			0, 1, 3, 6, 10, 15, 21, 28, 36, 45, 55, 66, 78, 91, 105, 
				120,136, 153, 171, 190, 210, 231, 253, 276, 300 }};
	for( const auto poly: polygonal_range<3, int>(1) )
	{
		if ( i > 24 ) break;
		EXPECT_EQ(poly, a[i]);

		i++;
	}
}

TEST(trigonal, infinite)
{
	//引数なし: 最初から
	int i = 0;
	static const std::array<int,25> a {{ 
			0, 1, 3, 6, 10, 15, 21, 28, 36, 45, 55, 66, 78, 91, 105, 
				120,136, 153, 171, 190, 210, 231, 253, 276, 300 }};
	for ( const auto poly: polygonal<3>() )
	{
		if ( i > 24 ) break;
		EXPECT_EQ(poly, a[i]);

		i++;
	}

	//途中から
	i = 5;
	for ( const auto poly: polygonal<3>(i) )
	{
		if ( i > 10 ) break;
		EXPECT_EQ(poly, a[i]);

		i++;
	}
}

TEST(trigonal, finite)
{
	//有限範囲
	int i = 10;
	static const std::array<int,25> a {{ 
			0, 1, 3, 6, 10, 15, 21, 28, 36, 45, 55, 66, 78, 91, 105, 
				120,136, 153, 171, 190, 210, 231, 253, 276, 300 }};
	for ( const auto poly: polygonal<3>(10, 20) )
	{
		EXPECT_EQ(poly, a[i]);
		i++;
	}
}


TEST(trigonal, index_of_value)
{
	if(auto i = range_detail::polygonal_iterator<3, int>::index_of(6))
		EXPECT_EQ(i.get(), 3U);
	else FAIL();

	if(auto i = range_detail::polygonal_iterator<3, int>::index_of(10))
		EXPECT_EQ(i.get(), 4U);
	else FAIL();

	if(auto i = range_detail::polygonal_iterator<3, int>::index_of(9))
		FAIL();
	
}
