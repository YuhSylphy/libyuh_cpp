#include <gtest/gtest.h>

#include "../include/yuh/fibonacci.hpp"

#include <vector>
#include <iterator>
#include <boost/range/algorithm.hpp>
#include <boost/range/combine.hpp>

using namespace yuh;

TEST(fibonacci_iterator, ctor_raw_values) 
{
	range_detail::fibonacci_iterator<int> it(0, 1);

	EXPECT_EQ(*it++, 0);
	EXPECT_EQ(*it++, 1);
	EXPECT_EQ(*it++, 1);
	EXPECT_EQ(*it++, 2);
	EXPECT_EQ(*it++, 3);
	EXPECT_EQ(*it++, 5);
	EXPECT_EQ(*it++, 8);
	EXPECT_EQ(*it++, 13);
	EXPECT_EQ(*it++, 21);
	EXPECT_EQ(*it++, 34);
	EXPECT_EQ(*it++, 55);

	EXPECT_EQ(*it--, 89);
	EXPECT_EQ(*it--, 55);
	EXPECT_EQ(*it--, 34);
	EXPECT_EQ(*it--, 21);
	EXPECT_EQ(*it--, 13);
	EXPECT_EQ(*it--, 8);
	EXPECT_EQ(*it--, 5);
	EXPECT_EQ(*it--, 3);
	EXPECT_EQ(*it--, 2);
	EXPECT_EQ(*it--, 1);
	EXPECT_EQ(*it--, 1);
	EXPECT_EQ(*it--, 0);

}

TEST(fibonacci_iterator, nth_fib)
{
	typedef range_detail::fibonacci_iterator<int> iter_type;
	EXPECT_EQ(iter_type::nth_fibonacci(1), 1);
	EXPECT_EQ(iter_type::nth_fibonacci(2), 1);
	EXPECT_EQ(iter_type::nth_fibonacci(3), 2);
	EXPECT_EQ(iter_type::nth_fibonacci(4), 3);
	EXPECT_EQ(iter_type::nth_fibonacci(5), 5);
	EXPECT_EQ(iter_type::nth_fibonacci(6), 8);
	EXPECT_EQ(iter_type::nth_fibonacci(7), 13);
	EXPECT_EQ(iter_type::nth_fibonacci(8), 21);
	EXPECT_EQ(iter_type::nth_fibonacci(9), 34);
	EXPECT_EQ(iter_type::nth_fibonacci(10), 55);
}

TEST(fibonacci_iterator, ctor_index)
{
	typedef range_detail::fibonacci_iterator<int> iter_type;
	EXPECT_EQ(*iter_type(1), 1);
	EXPECT_EQ(*iter_type(2), 1);
	EXPECT_EQ(*iter_type(3), 2);
	EXPECT_EQ(*iter_type(4), 3);
	EXPECT_EQ(*iter_type(5), 5);
	EXPECT_EQ(*iter_type(6), 8);
	EXPECT_EQ(*iter_type(7), 13);
	EXPECT_EQ(*iter_type(8), 21);
	EXPECT_EQ(*iter_type(9), 34);
	EXPECT_EQ(*iter_type(10), 55);
}

TEST(fibonacci_iterator, distance)
{
	typedef range_detail::fibonacci_iterator<int> iter_type;
	
	EXPECT_EQ(std::distance(iter_type(1), iter_type(4)), 3);
	EXPECT_EQ(std::distance(iter_type(5), iter_type(5)), 0);
	// EXPECT_EQ(std::distance(iter_type(3), iter_type(2)), -1); //無限ループ
}

TEST(fibonacci_range, ctor_finite)
{
	const auto check = [](
		const fibonacci_range<int>& tgt,
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

	check(fibonacci_range<int>(0, 10),
		  {0,1,1,2,3,5,8,13,21,34}
		);
	check(fibonacci_range<int>(10, 15),
		  {55,89,144,233,377}
		);
}

TEST(fibonacci_range, ctor_infinite)
{
	int i = 0;
	for( const auto fib: fibonacci_range<int>(0) )
	{
		if ( i > 30 ) break;
		EXPECT_EQ(fib, range_detail::fibonacci_iterator<int>::nth_fibonacci(i));

		i++;
	}
}

TEST(fibonacci, infinite)
{
	//引数なし: 最初から
	int i = 0;
	for ( const auto fib: fibonacci() )
	{
		if ( i > 30 ) break;
		EXPECT_EQ(fib, range_detail::fibonacci_iterator<int>::nth_fibonacci(i));

		i++;
	}

	//途中から
	i = 5;
	for ( const auto fib: fibonacci(i) )
	{
		if ( i > 10 ) break;
		EXPECT_EQ(fib, range_detail::fibonacci_iterator<int>::nth_fibonacci(i));

		i++;
	}
}

TEST(fibonacci, finite)
{
	//有限範囲
	int i = 10;
	for ( const auto fib: fibonacci(10, 20) )
	{
		EXPECT_EQ(fib, range_detail::fibonacci_iterator<int>::nth_fibonacci(i));
		i++;
	}
}
