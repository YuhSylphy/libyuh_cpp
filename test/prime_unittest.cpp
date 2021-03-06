#include <gtest/gtest.h>

#include "../include/yuh/prime.h"

#include <vector>
#include <set>
#include <iterator>
#include <functional>
#include <boost/range/algorithm.hpp>
#include <boost/range/numeric.hpp>
#include <boost/range/combine.hpp>
#include <boost/range/irange.hpp>

using namespace yuh;

TEST(prime_iter, init)
{
	prime_sift(1000);
}

TEST(prime_iter, in_de_crement) 
{
	range_detail::prime_iterator it;

	
	EXPECT_EQ(*it++, 2);
	EXPECT_EQ(*it++, 3);
	EXPECT_EQ(*it++, 5);
	EXPECT_EQ(*it++, 7);
	EXPECT_EQ(*it++, 11);
	
	EXPECT_EQ(*it--, 13);
	EXPECT_EQ(*it--, 11);
	EXPECT_EQ(*it--, 7);
	EXPECT_EQ(*it--, 5);
	EXPECT_EQ(*it--, 3);
	EXPECT_EQ(*it--, 2);
}

TEST(prime_iter, get_prime)
{
	const auto& p = range_detail::prime_iterator::get_prime();
	
	//EXPECT_EQ(p.size(), 168U);
	//EXPECT_EQ(p.back(), 997);
	EXPECT_EQ(p[101], 547);
}

TEST(prime_iter, approx)
{
	const auto approx = &range_detail::prime_iterator::prime_approx;
	const auto & p = range_detail::prime_iterator::get_prime();

	for( const auto i: boost::irange(1U, p.size()) )
	{
		EXPECT_GE(approx(i)*1.02, p[i]);
	}
}

TEST(prime_iter, expand)
{
	range_detail::prime_iterator it(1501);

	EXPECT_EQ(*it++, 12569);
	EXPECT_EQ(*it, 12577);
	
	auto it2 = it;
	std::advance(it, 3);
	
	EXPECT_EQ(*it2, 12577);
	EXPECT_EQ(*it, 12601);
	EXPECT_EQ(std::distance(it2, it), 3);
}

TEST(prime_iter, huge)
{
	prime_sift(1000000);
	//std::cout << range_detail::prime_iterator::get_prime().size() << std::endl;
}

TEST(prime_iter, prime_list)
{
	EXPECT_EQ(get_prime_list()[101], 547);
}

TEST(prime_iter, finite)
{
	const auto rng = boost::combine(
		prime_range(1, 11),
		std::vector<long long>{2,3,5,7,11,13,17,19,23,29}
		);
	for( const auto& t: rng )
	{
		EXPECT_EQ(boost::get<0>(t), boost::get<1>(t));
	}
}

TEST(prime_iter, infinite)
{
	std::vector<long long> v{2,3,5,7,11,13,17,19,23,29};
		
	int i = 0;
	for ( const auto& p: prime_range(1) )
	{
		if(i >= 10) break;
		EXPECT_EQ(p, v[i]);
		i++;
	}
}

TEST(prime, finite)
{
	const auto rng = boost::combine(
		prime(1, 11),
		std::vector<long long>{2,3,5,7,11,13,17,19,23,29}
		);
	for( const auto& t: rng )
	{
		EXPECT_EQ(boost::get<0>(t), boost::get<1>(t));
	}
}

TEST(prime, infinite)
{
	std::vector<long long> v{2,3,5,7,11,13,17,19,23,29};
		
	int i = 0;
	for ( const auto& p: prime() )
	{
		if(i >= 10) break;
		EXPECT_EQ(p, v[i]);
		i++;
	}
}

TEST(factor, test)
{
	auto f1 = prime_factor(13195);
	std::multiset<int> v1{5, 7, 13, 29};

	for ( const auto t: boost::combine(f1, v1) )
	{
		EXPECT_EQ(boost::get<0>(t), boost::get<1>(t));
	}

	std::multiset<int> v2{2, 2, 3, 5, 19, 773};
	auto f2 = prime_factor(
		boost::accumulate(v2, 1, std::multiplies<int>()));
	for ( const auto t: boost::combine(f2, v2) )
	{
		EXPECT_EQ(boost::get<0>(t), boost::get<1>(t));
	}

}

