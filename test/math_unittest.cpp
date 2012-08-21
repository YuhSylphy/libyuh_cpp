#include <gtest/gtest.h>

#include "../include/yuh/math.hpp"

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
}
