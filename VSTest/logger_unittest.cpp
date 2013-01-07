#include "stdafx.h"
#include "CppUnitTest.h"

#include <yuh/logger.h>

#include <yuh/adaptor/prettied.hpp>

#include <pstade/oven/initial_values.hpp>

#include <boost/range/irange.hpp>
#include <boost/format.hpp>
#include <future>

#include <string>
#include <array>
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace yuh::adaptors;

namespace oven = pstade::oven;
using namespace yuh::adaptors;

// #ifdef _DEBUG
namespace yuh
{
	namespace {
		void* enabler;
	}
}
// #endif

namespace VSTest
{
	TEST_CLASS(logger_unittest)
	{
	public:
		
		TEST_METHOD(logger_ctor)
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

		TEST_METHOD(logger_singleton)
		{
			std::array<std::string, 3> filenames = {
				"data/s1.txt",
				"data/s2.txt",
			};

			yuh::logger::init_emplace(filenames);
			{
				int const num = 100;
				auto test1 = [&](){
					for( auto i: boost::irange(0, num) )
					{
						yuh::logger::inst()(0, i);
					}
				};
				auto test2 = [&](){
					for( auto i: boost::irange(0, num) )
					{
						std::vector<int> buf = oven::initial_values(i, i+1);
						yuh::logger::inst()(1, i+1);
					}
				};

				auto f1 = std::async(test1);
				auto f2 = std::async(test2);
				//yuh::logger::inst()(0, std::string("test1"));
				//yuh::logger::inst()(0, std::string("test2"));
				//yuh::logger::inst()(0, std::string("test3"));
				//yuh::logger::inst()(0, std::string("test4"));
				//yuh::logger::inst()(1, std::string(""));
				//yuh::logger::inst()(1, 'a');
				//yuh::logger::inst()(1, std::string(""));
				//yuh::logger::inst()(1, 'b');
				//yuh::logger::inst()(1, std::string(""));
			}

			yuh::logger::reset();
		}
	};
}