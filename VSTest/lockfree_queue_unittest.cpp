#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <yuh/any.hpp>

#include <boost/lockfree/queue.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/irange.hpp>
#include <boost/format.hpp>

#include <vector>
#include <future>
#include <iostream>

#include "test_logger.h"

#ifndef _DEBUG


namespace lockfree = boost::lockfree;

namespace VSTest
{
	TEST_CLASS(lockfree_queue_unittest)
	{
	public:
		
		TEST_METHOD(lockfree_queue)
		{
			// TODO: テスト コードをここに挿入します

			lockfree::queue<yuh::any> q;

			size_t const len = 250;
			size_t const N = 3;
			std::vector<yuh::any> vec;
			vec.reserve(len);

			auto string = std::async([&]{
				yuh::any a;
				boost::for_each(
					boost::irange(0U, len),
					[&](int i){
						a = (boost::format("s%||") % i).str();
						q.push(a);
					});
			});

			auto floating = std::async([&]{
				yuh::any a;
				boost::for_each(
					boost::irange(0U, len),
					[&](int i){
						a = static_cast<double>(i) / len;
						q.push(a);
					});
			});

			auto integer = std::async([&]{
				yuh::any a;
				boost::for_each(
					boost::irange(0U, len),
					[&](int i){
						a = i;
						q.push(a);
					});
			});

			auto accum = std::async([&]{
				yuh::any buf;
				while(vec.size() < len * N)
				{
					if( q.pop(buf) )
					{
						vec.push_back(buf);
					}
				}
			});

			integer.get();
			floating.get();
			string.get();
			accum.get();

			//tcout << boost::format("size: %||\n") % vec.size();
			//boost::for_each(
			//	vec,
			//	[](yuh::any const& a){ tcout << a << " "; }
			//);
		}

	};
}

#endif
