#include <gtest/gtest.h>

#include "../include/yuh/math.hpp"

using namespace yuh;

TEST(math, palindromic)
{
	EXPECT_EQ(is_palindromic(9009), true);
	EXPECT_EQ(is_palindromic(9109), false);
	EXPECT_EQ(is_palindromic(109), false);
}
