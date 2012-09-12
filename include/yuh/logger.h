#pragma once

#include "thread_queue.hpp"
#include "any.hpp"
#include "singleton.hpp"


#include <boost/range.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/filesystem.hpp>

#include <thread>
#include <atomic>
#include <chrono>
#include <fstream>

//ユニットテスト用
namespace VSTest
{
	class logger_unittest;
}


namespace yuh
{

	namespace detail
	{
		/**
		* ログ
		*/
		class logger
		{
		public:
			/*
			* デストラクタ
			*/
			~logger();

			/**
			* 出力queueにpush
			* @param n queue番号
			* @param data 出力データ ストリーム出力operator<<に対応していればなんでも
			* @return 自身 ()を連結していける
			*/
			template<typename T>
			logger& operator()(int n, T const& data);

		private:
			logger();
			/**
			* ctor
			* @param rng 出力先ファイル名リスト
			* @param interval_milliseconds 出力チェック間隔
			*/
			template<typename Range>
			logger(Range& rng, int interval_milliseconds=100);

			logger(const logger&);
			logger(logger&&);
			logger& operator=(const logger&);
			logger& operator=(logger&&);

			/**
			* ループ関数 bgで出力していく
			*/
			void thread_loop();
			/**
			* 内部用出力処理
			*/
			void output();

			/**
			* 出力用queueリスト
			*/
			std::vector<thread_queue<any> > q_;
			/**
			* 出力ストリームリスト
			*/
			std::vector<std::ofstream> ofs_;

			/**
			* チェック待機時間
			*/
			std::chrono::milliseconds interval_;
			/**
			* スレッド終了フラグ
			*/
			std::atomic<bool> end_flag_;
			/**
			* スレッド
			*/
			std::thread thread_;

			//テスト用
			friend class VSTest::logger_unittest;
			//シングルトン
			friend class singleton<logger>;
		};

		template<typename Range>
		logger::logger(Range& filenames, int interval_milliseconds)
			: q_(boost::size(filenames))
			, ofs_(0)
			, interval_(interval_milliseconds)
			, end_flag_(true)
			, thread_()
		{
			ofs_.reserve(q_.size());
			boost::for_each(
				filenames, 
				[&](std::string const& str)	{ 
					auto p = boost::filesystem::absolute(boost::filesystem::path(str));
					if ( !boost::filesystem::exists(p.parent_path()) )
						boost::filesystem::create_directories(p.parent_path());

					ofs_.emplace_back(p.string()); 
			}
			);

			thread_ = std::thread(&logger::thread_loop, this); 
		}

		template<typename T>
		logger& logger::operator()(int n, T const& data)
		{
			q_[n].push(data);
			return *this;
		}
	} // namespace detail
	/**
	 * シングルトンオブジェクト
	 */
	typedef singleton<detail::logger> logger;
} // namespace yuh
