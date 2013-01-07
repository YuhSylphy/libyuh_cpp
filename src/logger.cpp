#include "stdafx.h"

#include "../include/yuh/logger.h"

#include <yuh/adaptor/prettied.hpp>

#include <boost/range/combine.hpp>

#include <iostream>
#include <sstream>
#include <mutex>

namespace yuh
{
	namespace detail
	{
		logger::logger()
			: q_()
			, ofs_()
			, interval_(100)
			, end_flag_(true)
			, thread_()
		{
			thread_ = boost::thread(&logger::thread_loop, this); //move
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

		bool logger::output()
		{
			any buf;
			std::stringstream ss;
			int const num = 20;
			bool ret = true;;
			for( auto t: boost::combine(q_, ofs_) )
			{
				auto& q = boost::get<0>(t);
				auto& ofs = boost::get<1>(t);
				if( !q.empty() )
				{
					ret = false; // 何かしら見つけたのでintervalは必要ない

					while( !q.empty() ) // 空になるまで
					{
						// ofs << q.front() << std::endl;
						ss << (q.front()/** | yuh::adaptors::prettied*/)  << std::endl; // stringとして保存
						q.pop();
					}

					//for( int i=0; i<num || q.pop(buf); i++ )
					//{
					//	ss << buf;
					//}

					// 一気に書き込む
					ofs.write(ss.str().c_str(), ss.str().size());
					ss.str("");
				}
			}
			return ret;
		}

		void logger::thread_loop()
		{
			while(end_flag_)
			{
				//メイン処理
				if ( output() )
				{
					boost::this_thread::sleep_for(interval_);
				}
			}
		}
	}
}

