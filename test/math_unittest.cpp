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
