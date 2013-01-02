#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define BOOST_RESULT_OF_USE_DECLTYPE

#include <product.hpp>

#include <pstade/oven/initial_values.hpp>

#include <boost/tuple/tuple_io.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/irange.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/numeric.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/adaptor/regular_extension/transformed.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/regular_extension/filtered.hpp>
#include <boost/format.hpp>

#include <iostream>
#include <string>
#include <vector>

#include "test_logger.h"

namespace oven = pstade::oven;
using namespace boost::adaptors;

namespace VSTest
{
	struct
	{
		template<typename T> 
		void operator()(T&& t) const
		{
			for( auto x: t )
			{
				 tcout << boost::format("%|| ") % x;
			}
			tcout << "\n";
		}
		template<typename T, typename Func> 
		void operator()(T&& t, Func f) const
		{
			for( auto x: t )
			{
				 tcout <<f(x);
			}
			tcout << "\n";
		}
	} range_view;


	TEST_CLASS(Misc)
	{
	public:
		//TEST_METHOD(misc_product)
		//{
		//	// TODO: テスト コードをここに挿入します

		//	 (1 a) (1 b) (1 c) (2 a) (2 b) (2 c) (3 a) (3 b) (3 c) 
		//	boost::for_each(
		//		boost::irange(1, 4) | iorate::product(boost::irange('a', 'd')),
		//		tcout << boost::lambda::_1 << ' '
		//		);
		//}


		TEST_METHOD(misc_transformed)
		{
			std::vector<int> vi = oven::initial_values(1,2,3,4,5);

			range_view(vi);
			range_view(vi | transformed([](int i){ return i+1; }));

			std::vector<std::tuple<int, int>> vt = oven::initial_values(
				std::make_tuple(1, 2),
				std::make_tuple(3, 4),
				std::make_tuple(5, 6),
				std::make_tuple(7, 8),
				std::make_tuple(9, 0)
				);
			auto t_view = [](std::tuple<int, int> const& x){ return boost::format("(%||, %||) ") % std::get<0>(x) % std::get<1>(x); };
			range_view(vt, t_view);
			range_view(vt | transformed([](std::tuple<int, int> t){
				std::get<0>(t)++;
				std::get<1>(t)++;
				return t; 
				}), t_view);


			auto const vec = [](int x, int y){
				std::vector<int> ret = oven::initial_values(x, y);
				return ret;
			};
			auto tv_view = [](std::tuple<std::vector<int>, int> const& x)
			{ return boost::format("({%||, %||}, %||) ") % std::get<0>(x)[0] % std::get<0>(x)[1] % std::get<1>(x); };
			std::vector<std::tuple<std::vector<int>, int>> vtv = oven::initial_values(
				std::make_tuple(vec(0, 1), 2),
				std::make_tuple(vec(3, 4), 5),
				std::make_tuple(vec(6, 7), 8)
				);
			range_view(vtv, tv_view);
			range_view(vtv | transformed([](std::tuple<std::vector<int>, int> t){
				std::get<0>(t)[0]++;
				std::get<0>(t)[1]++;
				std::get<1>(t)++;
				return t; 
				}),	tv_view);

		}
	};
}

