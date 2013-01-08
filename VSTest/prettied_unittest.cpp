#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <yuh/any.hpp>
#include <yuh/adaptor/prettied.hpp>

#include <pstade/oven/initial_values.hpp>

#include <boost/range/has_range_iterator.hpp>

#include <sstream>

#include "test_logger.h"
//
namespace yuh
{
	namespace {
		void* enabler;
	}
}

namespace VSTest
{
	namespace oven=pstade::oven;
	using namespace yuh::adaptors;
	extern void* enabler;
	template<class T>
	typename std::enable_if<!boost::has_range_const_iterator<T>::value>::type 
		f(T const& t)
	{
		tcout 
			<< typeid(std::enable_if<!boost::has_range_const_iterator<T>::value>::type*&).name() << " "
			<< typeid(T).name() << ": 非Range\n";
	}
	template<class T>
	typename std::enable_if<boost::has_range_const_iterator<T>::value>::type
		f(T const& t)
	{
		tcout 
			<< typeid(std::enable_if<boost::has_range_const_iterator<T>::value>::type*&).name() << " "
			<< typeid(T).name() << ": Range\n";
	}
	//template<typename Range
	//	, typename ::std::enable_if<boost::has_range_const_iterator<Range>::value>::type *& = enabler
	//>
	//void f(Range const& r)
	//{
	//	tcout << typeid(Range).name() << ": Range\n";
	//}
	//
	template<class T, typename std::enable_if<std::is_same<T, int>::value>::type*& = enabler>
	void g(T const &)
	{
		tcout << typeid(T).name() << ": g(int)\n";
	}


	TEST_CLASS(prettied_unittest)
	{
	public:

		TEST_METHOD(prettied_functest)
		{
			int i = 0;
			std::vector<int> vec = oven::initial_values(1, 2, 3);

			f(i);
			f(vec);

			//g(i);
			//g(vec);

			tcout << boost::has_range_const_iterator<int>::value << "\n";
			tcout << boost::has_range_const_iterator<std::vector<int>>::value << "\n";

			tcout << typeid(std::enable_if<1>::type*&).name() << "\n";
			// tcout << typeid(std::enable_if<0>::type).name() << "\n";

			//tcout << typeid(std::enable_if<boost::has_range_const_iterator<int>::value>::type*&).name() << "\n";
			tcout << typeid(std::enable_if<!boost::has_range_const_iterator<int>::value>::type*&).name() << "\n";
			tcout << typeid(std::enable_if<boost::has_range_const_iterator<std::vector<int>>::value>::type*&).name() << "\n";
			//tcout << typeid(std::enable_if<!boost::has_range_const_iterator<std::vector<int>>::value>::type*&).name() << "\n";
		}
		

		TEST_METHOD(prettied_default)
		{
			// TODO: テスト コードをここに挿入します

			std::stringstream ss;
			std::vector<int> vec = oven::initial_values(1, 2, 3);
			int i=0;
			yuh::any a = 3.14;
			
			ss << (vec | prettied);
			Assert::AreEqual(std::string("{ 1, 2, 3 }"), ss.str());
			ss.str("");

			ss << (i | prettied);
			Assert::AreEqual(std::string("0"), ss.str());
			ss.str("");

			ss << (a | prettied);
			Assert::AreEqual(std::string("3.14"), ss.str());
			ss.str("");

		}

		TEST_METHOD(prettied_format)
		{

			std::stringstream ss;
			std::vector<int> vec = oven::initial_values(1, 2, 3);
			int i=0;
			yuh::any a = 3.14;

			prettied.format("", ",", "");
			
			ss << (vec | prettied);
			Assert::AreEqual(std::string("1,2,3"), ss.str());
			ss.str("");

			ss << (i | prettied);
			Assert::AreEqual(std::string("0"), ss.str());
			ss.str("");

			ss << (a | prettied);
			Assert::AreEqual(std::string("3.14"), ss.str());
			ss.str("");

			prettied.format();

		}

		TEST_METHOD(prettied_operator)
		{

			std::stringstream ss;
			std::vector<int> vec = oven::initial_values(1, 2, 3);
			int i=0;
			yuh::any a = 3.14;

			ss << (vec | prettied("", ",", ""));
			Assert::AreEqual(std::string("1,2,3"), ss.str());
			ss.str("");

			ss << (i | prettied("", ",", ""));
			Assert::AreEqual(std::string("0"), ss.str());
			ss.str("");

			ss << (a | prettied("", ",", ""));
			Assert::AreEqual(std::string("3.14"), ss.str());
			ss.str("");

		}

		TEST_METHOD(prettied_string)
		{
			std::stringstream ss;
			std::string str("tester");
			std::vector<std::string> vec = oven::initial_values(
				std::string("one"),
				std::string("two"),
				std::string("three")
				);

			ss << (str | prettied);
			Assert::AreEqual(std::string("tester"), ss.str());
			ss.str("");

			ss << (vec | prettied);
			Assert::AreEqual(std::string("{ one, two, three }"), ss.str());
			ss.str("");
		}

	};
}