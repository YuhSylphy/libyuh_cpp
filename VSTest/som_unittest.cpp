#include "stdafx.h"
#include "CppUnitTest.h"

#include "../include/yuh/som.hpp"

#include <yuh/combine.hpp>

#include <pstade/oven/initial_values.hpp>

#include <array>
#include <random>




using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace oven = pstade::oven;

typedef std::array<int, 8> Input;
typedef std::array<double, 8> Ref;
typedef yuh::som<Input, Ref> som;

namespace VSTest
{
	TEST_CLASS(som_unittest)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			// TODO: テスト コードをここに挿入します
			som s(
				oven::initial_values(30, 30), 
				[](){ return Ref(); }, 
				[](Input const&, Ref const&){ return .0; },
				[](int, std::vector<int> const&){ return std::vector<std::tuple<std::vector<int>, double>>(); },
				[](Ref const&, float, int, Input const&){ return Ref(); }
			);

			Assert::AreEqual(900U, s.refs().size());
		}

		TEST_METHOD(RadConv_n_to_inf)
		{
			std::vector<int> v = oven::initial_values(2,3,5);

			int n = 7;
			auto expected = 2 * static_cast<int>(std::pow(n, v[0])) + v[1] * n + v[2];
			auto actual = som::rad_conv(v, n);

			Assert::AreEqual(expected, actual);
		}
		TEST_METHOD(RadConv_inf_to_n)
		{
			std::vector<int> v = oven::initial_values(2,3,5);

			int n = 7;
			int x = 2 * static_cast<int>(std::pow(n, v[0])) + v[1] * n + v[2];

			auto& expected = v;
			auto actual = som::rad_conv(x, n);

			for ( auto const& t: yuh::combine(expected, actual) )
			{
				auto const& e = boost::get<0>(t);
				auto const& a = boost::get<1>(t);

				Assert::AreEqual(e, a);
			}
		}

	};
}
