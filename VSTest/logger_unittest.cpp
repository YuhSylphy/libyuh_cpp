#include "stdafx.h"
#include "CppUnitTest.h"

#include <yuh/logger.h>

#include <yuh/adaptor/prettied.hpp>


#include <string>
#include <array>
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace yuh::adaptors;

#ifdef _DEBUG
namespace yuh
{
	namespace {
		void* enabler;
	}
}
#endif

namespace VSTest
{
	TEST_CLASS(logger_unittest)
	{
	public:
		
		TEST_METHOD(ctor)
		{
			// TODO: テスト コードをここに挿入します

			std::array<std::string, 3> filenames = {
				"data/file1.txt",
				"data/file2.txt",
				"data/file3.txt",
			};
			{
				yuh::detail::logger logger(filenames);

				logger(0, 3)(0, 3.14)(0, std::string("pi"));
				logger(1, std::string("test"));
				logger(2, filenames | prettied("", ",", ""));
			}



			std::ifstream ifs(filenames[0]);
			std::string buf;
			std::getline(ifs, buf);
			Assert::AreEqual(std::string("3"), buf);
			std::getline(ifs, buf);
			Assert::AreEqual(std::string("3.14"), buf);
			std::getline(ifs, buf);
			Assert::AreEqual(std::string("pi"), buf);
			ifs.close();

			ifs.open(filenames[1]);
			std::getline(ifs, buf);
			Assert::AreEqual(std::string("test"), buf);
			ifs.close();

		}

		TEST_METHOD(singleton)
		{
			std::array<std::string, 3> filenames = {
				"data/s1.txt",
				"data/s2.txt",
			};

			yuh::logger::init_emplace(filenames);
			{
				yuh::logger::inst()(0, std::string("test1"));
				yuh::logger::inst()(0, std::string("test2"));
				yuh::logger::inst()(0, std::string("test3"));
				yuh::logger::inst()(0, std::string("test4"));
				yuh::logger::inst()(1, std::string(""));
			}

			yuh::logger::reset();
		}
	};
}