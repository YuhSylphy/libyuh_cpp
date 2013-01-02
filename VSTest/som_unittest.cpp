#include "stdafx.h"
#include "CppUnitTest.h"

#include "../include/yuh/som.hpp"

#define BOOST_RESULT_OF_USE_DECLTYPE

#include "test_logger.h"

#include <yuh/combine.hpp>

#include <pstade/oven/initial_values.hpp>

#include <boost/range/algorithm.hpp>
#include <boost/range/numeric.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/adaptor/regular_extension/transformed.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/regular_extension/filtered.hpp>
#include <boost/range/experimental/as_container.hpp>
#include <boost/algorithm/cxx11/all_of.hpp>

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
		
		TEST_METHOD(SOM_ctor)
		{
			// TODO: テスト コードをここに挿入します
			som s(
				oven::initial_values(30, 30), 
				[](){ return Ref(); }, 
				[](Input const&, Ref const&){ return .0; },
				[](int){ return std::vector<std::tuple<std::vector<int>, double>>(); },
				[](Ref const&, double, int, Input const&){ return Ref(); }
			);

			Assert::AreEqual(900U, s.refs().size());
		}

		TEST_METHOD(RadConv_n_to_inf)
		{
			std::vector<int> v = oven::initial_values(2,3,5);

			std::vector<int> n = oven::initial_values(7,7,7);
			auto expected = v[2] * n[1] * n[0] + v[1] * n[0] + v[0];
			auto actual = som::rad_compress(v, n);

			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(RadConv_inf_to_n)
		{
			std::vector<int> v = oven::initial_values(2,3,5);

			std::vector<int> n = oven::initial_values(7,7,7);
			int x = v[2] * n[1] * n[0] + v[1] * n[0] + v[0];

			auto& expected = v;
			auto actual = som::rad_extract(x, n);

			Assert::AreEqual(boost::distance(expected), boost::distance(actual));
			for ( auto const& t: boost::combine(expected, actual) )
			{
				auto const& e = boost::get<0>(t);
				auto const& a = boost::get<1>(t);

				Assert::AreEqual(e, a);
			}
		}

		TEST_METHOD(Step)
		{
			som s(
				oven::initial_values(30, 30), 
				[](){ return Ref(); }, 
				som::euc_dist,
				som::rect_neighbor,
				[](Ref const&, double, int, Input const&){ return Ref(); }
			);
			std::array<int,8> i = {{0,0,1,1,3,3,5,6}};
			s.step(i);
		}

		TEST_METHOD(Euclidean_distance)
		{
			Input x = {{1,2,3,0,0,0,0}};
			Ref y = {{4,5,6,0,0,0,0}};

			Assert::AreEqual(
				std::sqrt(27),
				som::euc_dist(x, y),
				.01
				);
		}
		TEST_METHOD(Rect_neighbor)
		{
			double const angle = .05;
			double const touched = .1;
			double const just = .4;

			auto const vec = [](int x, int y){
				std::vector<int> ret = oven::initial_values(x, y);
				return ret;
			};

			auto actual = som::rect_neighbor(0);
			std::vector<std::tuple<std::vector<int>, double>> expected
				= oven::initial_values(
					std::make_tuple(vec( -1, -1 ), angle),
					std::make_tuple(vec( -1,  0 ), touched),
					std::make_tuple(vec( -1,  1 ), angle),
					std::make_tuple(vec(  0, -1 ), touched),
					std::make_tuple(vec(  0,  0 ), just),
					std::make_tuple(vec(  0,  1 ), touched),
					std::make_tuple(vec(  1, -1 ), angle),
					std::make_tuple(vec(  1,  0 ), touched),
					std::make_tuple(vec(  1,  1 ), angle)
					);


			//tcout << "actual\n";
			//for( auto &t: actual)
			//{
			//	tcout << boost::format("( %|2|, %|2| ): %|5|\n") % std::get<0>(t)[0] % std::get<0>(t)[1] % std::get<1>(t);
			//}
			//tcout << "expected\n";
			//for( auto &t: expected)
			//{
			//	tcout << boost::format("( %|2|, %|2| ): %|5|\n") % std::get<0>(t)[0] % std::get<0>(t)[1] % std::get<1>(t);
			//}


			for( auto& t: boost::combine(actual,expected) )
			{
				auto& act = boost::get<0>(t);
				auto& exp = boost::get<1>(t);

				//tcout << std::get<0>(act)[0] << ", " << std::get<0>(act)[1] << "\n";
				//tcout << std::get<0>(exp)[0] << ", " << std::get<0>(exp)[1] << "\n";

				Assert::AreEqual(std::get<0>(exp)[0], std::get<0>(act)[0]);
				Assert::AreEqual(std::get<0>(exp)[1], std::get<0>(act)[1]);
				Assert::AreEqual(std::get<1>(exp), std::get<1>(act), .001);

			}

		}

		TEST_METHOD(cut_neighbor)
		{
			using namespace boost::adaptors;
			auto th = som::rect_neighbor(0);
			std::vector<int> dim = oven::initial_values(30, 30);
			auto f = [&](std::vector<int> pos) -> std::vector<std::tuple<std::vector<int>, double>>
			{
				return th
					| transformed([&](std::tuple<std::vector<int>, double> t) {
						auto& v = std::get<0>(t);
						boost::for_each(boost::combine(v, pos),
							[](boost::tuple<int&, int> s) {
								boost::get<0>(s) += boost::get<1>(s);
							});
						return t;
					} )
					| filtered([&](std::tuple<std::vector<int>, double> t)
					{
						auto& v = std::get<0>(t);
						return boost::algorithm::all_of(
							boost::combine(v, dim),
							[](boost::tuple<int, int> s) {
								return 0 <= boost::get<0>(s) && boost::get<0>(s) < boost::get<1>(s);
							}
							);
					} )
					| boost::as_container;
					;
			};

			auto g = [&](int x, int y){
				tcout << boost::format("%||, %||\n") % x % y;
				for( auto t: f(oven::initial_values(x,y)) )
				{
					auto const& v = std::get<0>(t);
					auto const& r = std::get<1>(t);
					tcout <<  boost::format("( %|2|, %|2| ): %|5|\n") % v[0] % v[1] % r;
				}
			};

			//g(0, 0);
			//g(3, 3);
			//g(29, 29);

		}
	};
}
