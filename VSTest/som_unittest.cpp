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
#include <boost/range/adaptor/taken.hpp>
#include <boost/range/as_container.hpp>
#include <boost/algorithm/cxx11/all_of.hpp>
#include <boost/range/iteration.hpp>
#include <boost/range/regular.hpp>



#include <array>
#include <random>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace oven = pstade::oven;

typedef std::array<int, 8> Input;
typedef std::array<double, 8> Ref;
typedef yuh::som<Input, Ref> som;

struct color
{
	color(double r, double g, double b)
		: R(r), G(g), B(b)
	{
	}

	color& normalize()
	{
		auto nrm = [](double& x)
		{
			if(x<0) x=.0;
			if(x>255) x=255.0;
		};
		nrm(R); nrm(G); nrm(B);
		return *this;
	}

	color& operator+=(color const& rhs)
	{
		R+=rhs.R;
		G+=rhs.G;
		B+=rhs.B;
		return *this;
	}
	color operator+(color const& rhs) const
	{
		return color(*this)+=rhs;
	}
	color& operator-=(color const& rhs)
	{
		R-=rhs.R;
		G-=rhs.G;
		B-=rhs.B;
		return *this;
	}
	color operator-(color const& rhs) const
	{
		return color(*this)-=rhs;
	}

	template<typename Num>
	color& operator*=(Num rhs)
	{
		R*=rhs;
		G*=rhs;
		B*=rhs;
		return *this;
	}
	template<typename Num>
	color operator*(Num rhs) const
	{
		return color(*this)*=rhs;
	}

	boost::format fmt() const
	{
		return boost::format("%|02X|%|02X|%|02X|") 
					% static_cast<int>(R)
					% static_cast<int>(G)
					% static_cast<int>(B)
					;
	}

	double R;
	double G;
	double B;

};

template<typename Num>
color operator*(Num lhs, color const& rhs)
{
	return color(rhs)*=lhs;
}



namespace VSTest
{
	TEST_CLASS(som_unittest)
	{
	public:
		
		TEST_METHOD(som_ctor)
		{
			// TODO: テスト コードをここに挿入します
			som s(
				oven::initial_values(30, 30), 
				[](){ return Ref(); }, 
				[](Input const&, Ref const&){ return .0; },
				[](int, uint64_t, std::vector<int>const&){ return std::vector<std::tuple<int, double>>(); },
				[](Ref const&, double, uint64_t, Input const&){ return Ref(); }
			);

			Assert::AreEqual(900U, s.refs().size());
		}

		TEST_METHOD(som_RadConv_compress)
		{
			std::vector<int> v = oven::initial_values(2,3,5);

			std::vector<int> n = oven::initial_values(7,7,7);

			som::set_rad_map(n);
			
			auto expected = v[2] * n[1] * n[0] + v[1] * n[0] + v[0];
			auto actual = som::rad_compress(v, n);

			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(som_RadConv_extract)
		{
			std::vector<int> v = oven::initial_values(2,3,5);

			std::vector<int> n = oven::initial_values(7,7,7);
			int x = v[2] * n[1] * n[0] + v[1] * n[0] + v[0];

			som::set_rad_map(n);

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

		TEST_METHOD(som_Step)
		{
			som s(
				oven::initial_values(30, 30), 
				[](){ return Ref(); }, 
				som::euc_dist,
				som::rect_neighbor,
				[](Ref const&, double, uint64_t, Input const&){ return Ref(); }
			);
			std::array<int,8> i = {{0,0,1,1,3,3,5,6}};
			s.step(i);
		}

		TEST_METHOD(som_Euclidean_distance)
		{
			Input x = {{1,2,3,0,0,0,0}};
			Ref y = {{4,5,6,0,0,0,0}};

			Assert::AreEqual(
				27.0,
				som::euc_dist(x, y),
				.01
				);
		}
		TEST_METHOD(som_Rect_neighbor)
		{
			double const angle = .05;
			double const touched = .1;
			double const just = .4;

			auto const vec = [](int x, int y){
				std::vector<int> ret = oven::initial_values(x, y);
				return ret;
			};

			auto test = [](
				std::vector<std::tuple<int, double>> const& expected,
				std::vector<std::tuple<int, double>> const& actual)
			{
				Assert::AreEqual(boost::distance(expected), boost::distance(actual));
				for( auto& t: boost::combine(expected, actual) )
				{
					auto& exp = boost::get<0>(t);
					auto& act = boost::get<1>(t);

					Assert::AreEqual(std::get<0>(exp), std::get<0>(act));
					Assert::AreEqual(std::get<1>(exp), std::get<1>(act), .001);
				}
			};

			{
				std::vector<int> dim = vec(30, 30);
				auto actual = som::rect_neighbor(0, 0, dim);
				std::vector<std::tuple<int, double>> expected
					= oven::initial_values(
						//std::make_tuple(som::rad_compress(vec( -1, -1 ), dim), angle),
						//std::make_tuple(som::rad_compress(vec( -1,  0 ), dim), touched),
						//std::make_tuple(som::rad_compress(vec( -1,  1 ), dim), angle),
						//std::make_tuple(som::rad_compress(vec(  0, -1 ), dim), touched),
						std::make_tuple(som::rad_compress(vec(  0,  0 ), dim), just),
						std::make_tuple(som::rad_compress(vec(  0,  1 ), dim), touched),
						//std::make_tuple(som::rad_compress(vec(  1, -1 ), dim), angle),
						std::make_tuple(som::rad_compress(vec(  1,  0 ), dim), touched),
						std::make_tuple(som::rad_compress(vec(  1,  1 ), dim), angle)
						);

				test(expected, actual);
			}
		}

		//TEST_METHOD(cut_neighbor)
		//{
		//	using namespace boost::adaptors;
		//	std::vector<int> dim = oven::initial_values(30, 30);
		//	auto th = som::rect_neighbor(0, 0, dim);
		//	auto f = [&](std::vector<int> pos) -> std::vector<std::tuple<int, double>>
		//	{
		//		return th
		//			| transformed([&](std::tuple<int, double> t) 
		//			{
		//				auto v = som::rad_extract(std::get<0>(t), dim);
		//				boost::for_each(boost::combine(v, pos),
		//					[](boost::tuple<int&, int> s) {
		//						boost::get<0>(s) += boost::get<1>(s);
		//					});
		//				return std::make_tuple(v, std::get<1>(t));
		//			} )
		//			| filtered([&](std::tuple<std::vector<int>, double> t)
		//			{
		//				auto& v = std::get<0>(t);
		//				return boost::algorithm::all_of(
		//					boost::combine(v, dim),
		//					[](boost::tuple<int, int> s) {
		//						return 0 <= boost::get<0>(s) && boost::get<0>(s) < boost::get<1>(s);
		//					});
		//			} )
		//			| transformed([&](std::tuple<std::vector<int>, double> t) 
		//			{
		//				return std::make_tuple(som::rad_compress(std::get<0>(t), dim), std::get<1>(t));
		//			} )
		//			| boost::as_container;
		//			;
		//	};

		//	auto g = [&](int x, int y){
		//		tcout << boost::format("%||, %||\n") % x % y;
		//		for( auto t: f(oven::initial_values(x,y)) )
		//		{
		//			auto const v = som::rad_extract(std::get<0>(t), dim);
		//			auto const& r = std::get<1>(t);
		//			tcout <<  boost::format("( %|2|, %|2| ): %|5|\n") % v[0] % v[1] % r;
		//		}
		//	};

		//	g(0, 0);
		//	g(3, 3);
		//	g(29, 29);

		//}

		TEST_METHOD(som_Quad_neighbor)
		{

			auto const vec = [](int x, int y){
				std::vector<int> ret = oven::initial_values(x, y);
				return ret;
			};
			som::set_rad_map(vec(7, 7));
			
			std::vector<int> dim = vec(7, 7);
			{
				auto th = som::quad_neighbor<1, 1, 10000>(0, 0, dim);
				for( auto t: th )
				{
					auto const v = som::rad_extract(std::get<0>(t), dim);
					auto const& r = std::get<1>(t);
					//tcout <<  boost::format("( %|2|, %|2| ): %|5|\n") % v[0] % v[1] % r;
				}
			}

		}

		TEST_METHOD(som_COLOR_SOM)
		{
			using namespace boost::adaptors;
			typedef yuh::som<color, color> colsom;

			std::mt19937 rnd(123456789);
			std::uniform_int_distribution<> const dst(0, 255);

			struct rc
			{
				rc(std::mt19937& rnd, std::uniform_int_distribution<> const& dst)
					: r(rnd), d(dst) {}

				inline color operator()(color const&) const
				{
					return (*this)();
				}
				inline color operator()() const
				{
					return color(d(r), d(r), d(r));
				}

				std::mt19937& r;
				std::uniform_int_distribution<>const&  d;
			} rndcol(rnd, dst);


			std::vector<int> dim = oven::initial_values(30, 30);

			auto html = [](std::vector<color> const& v)
			{
				const std::string str = 
					std::string("<style type=\"text/css\">") +
					"table{border-collapse:collapse;border-spacing:0;}" +
					"td{width:10px;height:10px;}" +
					"</style>"+
					"<table>"+
					boost::accumulate(
						boost::irange(0, 30), 
						std::string(), 
						[](std::string str, int) 
						{ 
							return str 
								+ "<tr>"
								+ boost::accumulate(
									boost::irange(0, 30), 
									std::string(), 
									[](std::string str, int) 
									{ 
										return str + "<td style=\"background-color: #%||\" />";
									} ) 
								+ "</tr>";
						} ) +
					"</table>";


				// boost::for_each(v, [](color const& c){ tcout << c.fmt() << "\n"; });
				tcout << 
					boost::accumulate(
					v,
					boost::format(str),
					[](boost::format fmt, color const& c) { return fmt % c.fmt(); }
					);
			};
			{
				colsom s(
					dim, 
					rndcol, 
					[](color const& lhs, color const& rhs)
					{ 
						return std::sqrt(
							std::pow(lhs.R - rhs.R, 2) +
							std::pow(lhs.G - rhs.G, 2) +
							std::pow(lhs.B - rhs.B, 2)
							); 
					},
					colsom::quad_neighbor<1, 1, 20000>,
					[](color const& col, double r, uint64_t t, color const& input){ return (col + (input - col) * r).normalize(); }
				);

				//html(s.refs());
				//for( auto c: boost::iteration(rndcol(), boost::regular(rndcol)) | taken(10000))
				//{
				//	s.step(c);
				//}
				//html(s.refs());
				//for( auto c: boost::iteration(rndcol(), boost::regular(rndcol)) | taken(10000))
				//{
				//	s.step(c);
				//}
				//html(s.refs());
				//for( auto c: boost::iteration(rndcol(), boost::regular(rndcol)) | taken(10000))
				//{
				//	s.step(c);
				//}
				//html(s.refs());
				//for( auto c: boost::iteration(rndcol(), boost::regular(rndcol)) | taken(10000))
				//{
				//	s.step(c);
				//}
				//html(s.refs());
				//for( auto c: boost::iteration(rndcol(), boost::regular(rndcol)) | taken(10000))
				//{
				//	s.step(c);
				//}
				//html(s.refs());
			}
		}
	};

}
