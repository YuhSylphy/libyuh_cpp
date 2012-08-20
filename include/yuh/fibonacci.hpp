#pragma once

#include <cmath>

#include <boost/assert.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/iterator_range.hpp>

namespace yuh
{
	namespace range_detail	
	{
		/**
		 * フィボナッチ数列を走査するイテレータ
		 * @param Integer フィボナッチ数を表現する整数型
		 */
		template<typename Integer>
		class fibonacci_iterator
			: public boost::iterator_facade<
			fibonacci_iterator<Integer>,
			Integer,
			boost::bidirectional_traversal_tag,
			Integer,
			std::ptrdiff_t
			>
		{
			/**
			 * 基底クラス(iterator_facade)
			 */
			typedef boost::iterator_facade<
				fibonacci_iterator<Integer>,
				Integer,
				boost::bidirectional_traversal_tag,
				Integer,
				std::ptrdiff_t
				> base_t;
			
		public:
			/**
			 * 渡された値型
			 */
			typedef typename base_t::value_type value_type;
			/**
			 * イテレータの距離を示す型
			 */
			typedef typename base_t::difference_type difference_type;
			/**
			 * 参照を剥がした際の型
			 */
			typedef typename base_t::reference reference;
			
			/**
			 * 無限範囲終端イテレータ用
			 *
			 * 前後どちらに動かしても変化しない
			 */
			fibonacci_iterator(): a0_(0), a1_(0){}
			/**
			 * 内部表現の値を直接指定する
			 *
			 * @param a0 イテレータが示すフィボナッチ数そのもの
			 * @param a1 この次のイテレータが示すべきフィボナッチ数
			 */
 			fibonacci_iterator(value_type a0, value_type a1): 
				a0_(a0), a1_(a1) {}
			/**
			 * 指定インデックスのフィボナッチ数を示す
			 *
			 * @param index このイテレータが示すフィボナッチ数の添字
			 */
			explicit fibonacci_iterator(size_t index)
				: a0_(nth_fibonacci( index )),
				  a1_(nth_fibonacci( index + 1 ))
				{}

			/**
			 * n番目のフィボナッチ数を返す
			 * 0から0,1,1,2,3,5,...
			 *
			 * @param n フィボナッチ数a_nの添字
			 * @return フィボナッチ数a_n
			 */
			static value_type nth_fibonacci(size_t n)
			{
				if(n <= 0) return 0;
				if(n <= 2) return 1;

				static const auto sqrt5 = std::sqrt(5);
				const auto a = (1 + sqrt5) / 2;
				const auto b = (1 - sqrt5) / 2;

				return static_cast<value_type>(
					std::round(
						( std::pow(a, n) - std::pow(b, n) ) / sqrt5 
						)
					);
			}

		private:
			/**
			 * operator++で呼ばれる
			 */
            void increment()
            {
                const auto t = a1_;
				a1_ = a0_ + a1_;
				a0_ = t;
            }
			
			/**
			 * operator--で呼ばれる
			 */
            void decrement()
            {
                const auto t = a0_;
				a0_ = a1_ - a0_;
				a1_ = t;
            }

			/**
			 * 等値チェック
			 *
			 * @param rhs operator==の右辺に取る値
			 * @return 内部表現が等しければ真 
			 */
            bool equal(const fibonacci_iterator& rhs) const
            {
                return a0_ == rhs.a0_ && a1_ == rhs.a1_;
            }

			/**
			 * 参照剥がし
			 * 
			 * @return このイテレータが指すフィボナッチ数
			 */
            reference dereference() const
            {
                return a0_;
            }

			/**
			 * for iterator_facade
			 */
			friend class ::boost::iterator_core_access;
			/**
			 * イテレータが示すフィボナッチ数そのもの
			 */
			value_type a0_;
			/**
			 * @param a1 この次のイテレータが示すべきフィボナッチ数
			 */
			value_type a1_;
		
		};
	} // namespace range_detail

	/**
	 * フィボナッチ数列の範囲
	 * 無限数列・有限数列どちらも表現可能
	 * @param Integer フィボナッチ数を表現する整数型
	 */
    template<typename Integer>
    class fibonacci_range
        : public boost::iterator_range< range_detail::fibonacci_iterator<Integer> >
    {
		/**
		 * イテレータの型
		 */
        typedef range_detail::fibonacci_iterator<Integer> iterator_t;
		/**
		 * 基底クラス(iterator_range)
		 */
        typedef boost::iterator_range<iterator_t> base_t;
    public:
		/**
		 * 有限フィボナッチ数列 index指定 [first, last)
		 * @param first 範囲の始め(含む)
		 * @param last  範囲の終わり(含まない)
		 * @return n in [first, last)のフィボナッチ数列a_n
		 */
        fibonacci_range(Integer first, Integer last)
            : base_t(iterator_t(first), iterator_t(last))
        { }
		/**
		 * 無限フィボナッチ数列 index指定 [first, Inf)
		 * 単純にalgorithmに渡したりすると無限ループなので注意
		 * @param first 範囲の始め(含む)
		 * @return n in [first, Inf)のフィボナッチ数列a_n
		 */
        fibonacci_range(Integer first)
            : base_t(iterator_t(first), iterator_t())
        { }
		
    };


	/**
	 * 無限フィボナッチ数列 index指定 [first, Inf)
	 * 単純にalgorithmに渡したりすると無限ループなので注意
	 *
	 * @param first 範囲の始め(含む)
	 * @return n in [first, Inf)のフィボナッチ数列を表すrange
	 */
	template<typename Integer = int>
	fibonacci_range<Integer>
	fibonacci(Integer first = 0)
	{
		BOOST_ASSERT( first >= 0 );
		return fibonacci_range<Integer>(first);
	}
	
	/**
	 * 有限フィボナッチ数列 index指定 [first, last)
	 * @param first 範囲の始め(含む)
	 * @param last  範囲の終わり(含まない)
	 * @return n in [first, last)のフィボナッチ数列を表すrange
	 */
	template <typename Integer = int>
	fibonacci_range<Integer>
	fibonacci(Integer first, Integer last)
	{
		BOOST_ASSERT( first <= last );
		BOOST_ASSERT( first >= 0 );
		return fibonacci_range<Integer>(first, last);
	}
}
