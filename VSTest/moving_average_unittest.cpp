#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../include/yuh/moving_average.hpp"

#include <boost/range/irange.hpp>

#include <vector>

#include "test_logger.h"

namespace VSTest
{
	TEST_CLASS(moving_average_unittest)
	{
	public:
		
		TEST_METHOD(moving_average_out)
		{
			// TODO: テスト コードをここに挿入します

			std::vector<double> expected(10);

			yuh::moving_average_gen<double> mave(10);

			for( auto i: boost::irange(0, 10) )
			{
				tcout << static_cast<double>(i) << ", " << mave.next(static_cast<double>(i)) << "\n";
			}
		}

	};
}