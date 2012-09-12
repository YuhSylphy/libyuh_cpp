#include "stdafx.h"

#include "../include/yuh/logger.h"

#include <boost/range/combine.hpp>

#include <iostream>
#include <mutex>

namespace yuh
{
	namespace detail
	{
		logger::logger()
			: interval_(100)
			, end_flag_(true)
			, thread_()
		{
			thread_ = std::thread(&logger::thread_loop, this); //move
		}


		logger::~logger()		
		{
			end_flag_ = false;
			thread_.join(); //ここで停止

			//後処理
			output();
			boost::for_each(
				ofs_, 
				[](std::ofstream& ofs){ ofs.close(); }
			);
		}

		void logger::output()
		{
			for( auto t: boost::combine(q_, ofs_) )
			{
				auto& q = boost::get<0>(t);
				auto& ofs = boost::get<1>(t);
				while(!q.empty())
				{
					ofs << q.front() << std::endl;
					q.pop();
				}
			}
		}

		void logger::thread_loop()
		{
			while(end_flag_)
			{
				//メイン処理
				output();

				std::this_thread::sleep_for(interval_);
			}
		}
	}
}

