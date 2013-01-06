#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


#include "../include/yuh/selection.hpp"

#include <yuh/adaptor/prettied.hpp>

#include <pstade/oven/initial_values.hpp>

#include <boost/range/irange.hpp>
#include <boost/range/adaptor/indexed.hpp>
#include <boost/range/adaptor/taken.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/adaptor/regular_extension/transformed.hpp>
#include <boost/range/iteration.hpp>
#include <boost/range/regular.hpp>
#include <boost/format.hpp>

#include <random>

namespace oven = pstade::oven;
using namespace boost::adaptors;
namespace VSTest
{
	TEST_CLASS(selection_unittest)
	{
	public:
		
		TEST_METHOD(selection_roulette)
		{
			// TODO: テスト コードをここに挿入します

			std::mt19937 rnd(123456789);
			std::vector<int> count(5);

			for ( auto i: boost::irange(0, 50000) )
			{
				count[yuh::rouletteSel(
					oven::initial_values(.2,.2,.2,.2,.2)
					| indexed(0)
					, rnd).index()]++;
			}

			for( auto c: count )
			{
				Logger::WriteMessage((boost::format("%|| ") % c).str().c_str());
			}

		}

	};
}