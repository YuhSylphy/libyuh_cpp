#pragma once

#include <deque>
#include <boost/range/numeric.hpp>

namespace yuh
{
	template<typename T, typename Result=T>
	class moving_average_gen
	{
	private:
		//ÉfÅ[É^óÒ
		std::deque<T> series_;
		Result value_;
		T sum_;
		bool prefirst_;

	public:
		moving_average_gen(size_t size)
			: series_(size)
			, value_(Result())
			, sum_()
			, prefirst_(true)
		{ }

		Result const& getValue() const
		{
			return value_;
		}

		Result const& next(T const& t)
		{
			if(prefirst_)
			{
				series_.assign(series_.size(), t);
				sum_ = boost::accumulate(series_, T());
				prefirst_ = false;
			}
			else
			{
				sum_-=series_.back();
				series_.pop_back();
				series_.push_front(t);
				sum_+=t;
			}

			value_ = sum_ / series_.size();

			return value_;
		}

		void reset()
		{
			prefirst_ = true;
			value_ = Result();
		}
	};
}