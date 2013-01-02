#pragma once

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <boost/lexical_cast.hpp>
using boost::lexical_cast;


namespace VSTest
{
	struct test_logger
	{
		template<typename T>
		test_logger const& operator<<(T&& t) const
		{
			Microsoft::VisualStudio::CppUnitTestFramework::Logger::WriteMessage(lexical_cast<std::string>(t).c_str());
			return *this;
		}
		template<typename T>
		test_logger const& operator<<(T const& t) const
		{
			Microsoft::VisualStudio::CppUnitTestFramework::Logger::WriteMessage(lexical_cast<std::string>(t).c_str());
			return *this;
		}

	} extern tcout;
}