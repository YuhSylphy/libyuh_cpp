#include <gtest/gtest.h>

#include "../include/yuh/math.hpp"

#include <array>
#include <list>
#include <vector>

using namespace yuh;

TEST(math, palindromic)
{
	EXPECT_EQ(is_palindromic(9009), true);
	EXPECT_EQ(is_palindromic(9109), false);
	EXPECT_EQ(is_palindromic(109), false);
}

TEST(math, pethagorean)
{
	EXPECT_EQ(is_Pythagorean(3,4,5), true);
	EXPECT_EQ(is_Pythagorean(10, 24, 26), true);
	EXPECT_EQ(is_Pythagorean(2,4,10), false);

	// Range_based
	std::array<int, 3> ary{{3, 4, 5}};
	EXPECT_EQ(is_Pythagorean(ary), true);
	std::list<int> lst{10, 24, 26};
	EXPECT_EQ(is_Pythagorean(lst), true);
	std::vector<int> vec{2, 4, 10};
	EXPECT_EQ(is_Pythagorean(vec), false);

	EXPECT_ANY_THROW(is_Pythagorean(std::array<int, 2>{{0}}));
	EXPECT_ANY_THROW(is_Pythagorean(std::array<int, 4>{{0}}));

}

TEST(math, square)
{
	EXPECT_EQ(is_square(4), true);
	EXPECT_EQ(is_square(9), true);
	EXPECT_EQ(is_square(144), true);
	EXPECT_EQ(is_square(3), false);
	EXPECT_EQ(is_square(12), false);
}

TEST(math, combination)
{
	EXPECT_EQ(comb(4, 2), 6);
	EXPECT_EQ(comb(5, 2), 10);
	EXPECT_EQ(comb(40LL, 20LL), 137846528820LL);
	EXPECT_EQ(comb(39LLU, 19LLU), 68923264410LLU);
	EXPECT_EQ(comb(50LLU, 11LLU), 37353738800LLU);
	
	EXPECT_EQ(comb(4, 4), 1);

//	EXPECT_EQ(comb(10, 12), 0);
//	EXPECT_EQ(comb(10, -2), 0);
	//EXPECT_EQ(comb(-5, -2), 0);

}
