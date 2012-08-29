#include <gtest/gtest.h>

#include "../include/yuh/bignum.h"


using namespace yuh;

TEST(bignum, to_hex)
{
	bignum x(false, std::vector<uint32_t>{ 0x89ABCDEFU, 0x01234567U });

	// std::cout << x.hex() << std::endl; 
	EXPECT_EQ(x.hex(), "0123456789ABCDEF");
	bignum y(-123456789);
	
	EXPECT_EQ(y.hex(), "-075BCD15");
	// std::cout << y.hex() << std::endl;
}

TEST(bignum, lshift)
{
	bignum x(true, std::vector<uint32_t>{ 0x76543210U, 0xFEDCBA98U });
	x <<= 4;
	//std::cout << x.hex() << std::endl; 
	EXPECT_EQ(x.hex(), "-0000000FEDCBA98765432100");

	bignum y(1);
	y <<= 32;
//	std::cout << y.hex() << std::endl; 
	EXPECT_EQ(y.hex(), "0000000100000000");
//	std::cout << (y<<4).hex() << std::endl; 
	EXPECT_EQ((y << 4).hex(), "0000001000000000");
}

TEST(bignum, plus)
{
	bignum x(50);
	x += bignum(50);
	EXPECT_EQ(x.hex(), "00000064");
	// std::cout << x.hex() << std::endl;

	bignum y(100);
	y<<=32;
	EXPECT_EQ((y+x).hex(), "0000006400000064");
	// std::cout << (y+x).hex() << std::endl;
	// std::cout << (x+y).hex() << std::endl;

	bignum z(0xFFFFFFFFU);
	EXPECT_EQ((z+bignum(1)).hex(), "0000000100000000");
	// std::cout << (z+bignum(1)).hex() << std::endl;
	EXPECT_EQ((z+bignum(2)).hex(), "0000000100000001");
	// std::cout << (z+bignum(2)).hex() << std::endl;
}

TEST(bignum, rshift)
{
	bignum x(true, std::vector<uint32_t>{ 0x76543210U, 0xFEDCBA98U });
	x >>= 4;
	//std::cout << x.hex() << std::endl; 
	EXPECT_EQ(x.hex(), "-0FEDCBA987654321");

	bignum y = bignum(1)+=bignum(0xFFFFFFFFU);
	y >>= 32;
//	std::cout << y.hex() << std::endl; 
	EXPECT_EQ(y.hex(), "00000001");
//	std::cout << (y<<4).hex() << std::endl; 
	EXPECT_EQ((y >> 4).hex(), "00000000");
}

TEST(bignum, comp)
{
	bignum a(100); // 一桁+ 1
	bignum b(300); // 一桁+ 2
	bignum c(-500); // 一桁- 1
	bignum d(-700); // 一桁- 2
	bignum e=bignum(150)<<=32; // 二桁+ 1
	bignum f=(bignum(600)<<=32)+=bignum(300); // 二桁+ 2
	bignum g=bignum(-250)<<=32; // 二桁- 1
	bignum h=bignum(-700)<<=32; // 二桁- 2

	EXPECT_EQ(a <  a, false);
	EXPECT_EQ(a <= a, true);
	EXPECT_EQ(a >  a, false);
	EXPECT_EQ(a >= a, true);
	
	EXPECT_EQ(a <  b, true);
	EXPECT_EQ(a <= b, true);
	EXPECT_EQ(a >  b, false);
	EXPECT_EQ(a >= b, false);
	
	EXPECT_EQ(b <  a, false);
	EXPECT_EQ(b <= a, false);
	EXPECT_EQ(b >  a, true);
	EXPECT_EQ(b >= a, true);

	EXPECT_EQ(a <  c, false);
	EXPECT_EQ(a <= c, false);
	EXPECT_EQ(a >  c, true);
	EXPECT_EQ(a >= c, true);

	EXPECT_EQ(c <  a, true);
	EXPECT_EQ(c <= a, true);
	EXPECT_EQ(c >  a, false);
	EXPECT_EQ(c >= a, false);

	EXPECT_EQ(c <  c, false);
	EXPECT_EQ(c <= c, true);
	EXPECT_EQ(c >  c, false);
	EXPECT_EQ(c >= c, true);

	EXPECT_EQ(c <  d, false);
	EXPECT_EQ(c <= d, false);
	EXPECT_EQ(c >  d, true);
	EXPECT_EQ(c >= d, true);

	EXPECT_EQ(d <  c, true);
	EXPECT_EQ(d <= c, true);
	EXPECT_EQ(d >  c, false);
	EXPECT_EQ(d >= c, false);

	EXPECT_EQ(e <  e, false);
	EXPECT_EQ(e <= e, true);
	EXPECT_EQ(e >  e, false);
	EXPECT_EQ(e >= e, true);

	EXPECT_EQ(e <  f, true);
	EXPECT_EQ(e <= f, true);
	EXPECT_EQ(e >  f, false);
	EXPECT_EQ(e >= f, false);

	EXPECT_EQ(f <  e, false);
	EXPECT_EQ(f <= e, false);
	EXPECT_EQ(f >  e, true);
	EXPECT_EQ(f >= e, true);

	EXPECT_EQ(a <  e, true);
	EXPECT_EQ(a <= e, true);
	EXPECT_EQ(a >  e, false);
	EXPECT_EQ(a >= e, false);

	EXPECT_EQ(a <  g, false);
	EXPECT_EQ(a <= g, false);
	EXPECT_EQ(a >  g, true);
	EXPECT_EQ(a >= g, true);

	EXPECT_EQ(c <  g, false);
	EXPECT_EQ(c <= g, false);
	EXPECT_EQ(c >  g, true);
	EXPECT_EQ(c >= g, true);

	EXPECT_EQ(g <  g, false);
	EXPECT_EQ(g <= g, true);
	EXPECT_EQ(g >  g, false);
	EXPECT_EQ(g >= g, true);

	EXPECT_EQ(g <  h, false);
	EXPECT_EQ(g <= h, false);
	EXPECT_EQ(g >  h, true);
	EXPECT_EQ(g >= h, true);

	EXPECT_EQ(h <  g, true);
	EXPECT_EQ(h <= g, true);
	EXPECT_EQ(h >  g, false);
	EXPECT_EQ(h >= g, false);

}

TEST(bignum, minus)
{
	bignum x(150);
	x -= bignum(50);
	EXPECT_EQ(x.hex(), "00000064");
	// std::cout << x.hex() << std::endl;

	bignum y(100);
	y<<=32;
	y+=100;
	EXPECT_EQ((y-x).hex(), "0000006400000000");
	// std::cout << (y+x).hex() << std::endl;
	// std::cout << (x+y).hex() << std::endl;

	bignum z(1);
	z<<=32;
	EXPECT_EQ((z-bignum(1)).hex(), "FFFFFFFF");
	// std::cout << (z+bignum(1)).hex() << std::endl;
	EXPECT_EQ((z-bignum(2)).hex(), "FFFFFFFE");
	// std::cout << (z+bignum(2)).hex() << std::endl;
}

TEST(bignum, div_single)
{
	bignum x(3020);
	x/=30;
	EXPECT_EQ(x.hex(), "00000064");

	bignum y(false, std::vector<unsigned>{ 0x0U, 0x64U });
	EXPECT_EQ(y.hex(), "0000006400000000");
	y/=10;
	EXPECT_EQ(y.hex(), "0000000A00000000");
}

TEST(bignum, string)
{
	EXPECT_EQ(bignum("100").hex(), bignum(100).hex());

	std::string s = "37107287533902102798797998220837590246510135740250";
	EXPECT_EQ(bignum(s).to_string(), s);
}

TEST(bignum, user_defined_literal)
{
	EXPECT_EQ((100_B).to_string(), "100");
}

TEST(bignum, output)
{
	std::stringstream ss;

	ss << 123456789_B;
	EXPECT_EQ(ss.str(), "123456789");
	ss.str("");

	ss << -1234567890123456789_B;
	EXPECT_EQ(ss.str(), "-1234567890123456789");
	ss.str("");

	
}
