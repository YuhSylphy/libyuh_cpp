#pragma once

#include <cmath>

#include <boost/optional.hpp>
#include <boost/assert.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/iterator_range.hpp>

#include "math.hpp"

namespace yuh
{
	namespace range_detail	
	{
		/**
		 * 多角数列を走査するイテレータ
		 * @param Integer 多角数を表現する整数型
		 */
		template<size_t P, typename Integer>
		class polygonal_iterator
			: public boost::iterator_facade<
			polygonal_iterator<P, Integer>,
			Integer,
			boost::random_access_traversal_tag,
			Integer,
			std::ptrdiff_t
			>
		{
			/**
			 * 基底クラス(iterator_facade)
			 */
			typedef boost::iterator_facade
			<
				polygonal_iterator<P, Integer>,
				Integer,
				boost::random_access_traversal_tag,
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
			polygonal_iterator(): n_(0){}
			/**
			 * 内部表現の値を直接指定する
			 *
			 * @param a1 イテレータが示す多角数の添字(index)
			 */
			explicit polygonal_iterator(size_t n): 
				n_(n) {}
						
			/**
			 * 渡した値がP角数ならばその添字を返す
			 * @param val チェックする値
			 * @return P角数ならばそのindex，でなければ無効値
			 */
			static boost::optional<size_t> index_of(value_type val)
			{
				const auto D = (P-4)*(P-4) + 8 * (P-2) * val;
				if(is_square(D))
				{
					return ( static_cast<value_type>(std::sqrt(D)) + (P-4) ) / 2 * (P - 2);
				}
				else
				{
					return boost::none;
				}
			}

		private:
			/**
			 * operator++で呼ばれる
			 */
			void increment()
			{
				n_++;
			}
			
			/**
			 * operator--で呼ばれる
			 */
			void decrement()
			{
				n_--;
			}

			/**
			 * 等値チェック
			 *
			 * @param rhs operator==の右辺に取る値
			 * @return 内部表現が等しければ真 
			 */
			bool equal(const polygonal_iterator& rhs) const
			{
				return n_ == rhs.n_;
			}

			/**
			 * 参照剥がし
			 * 
			 * @return このイテレータが指す多角数
			 */
			reference dereference() const
			{
				return ((P-2)*n_*n_ - (P-4)*n_)/2;
			}

			/**
			 * std::advanceで呼ばれる
			 * @param offset 移動する目標との距離
			 */
			void advance(difference_type offset)
			{
				if(n_ <= -offset )
					n_ = 0;
				else
					n_ += offset;
			}

			/**
			 * std::distanceで呼ばれる
			 * @param rhs 比較対象
			 * @return 自身とrhsの距離
			 */
			difference_type distance_to(const polygonal_iterator& rhs)
			{
				return static_cast<std::ptrdiff_t>(rhs.n_) - n_;
			}

			/**
			 * for iterator_facade
			 */
			friend class ::boost::iterator_core_access;
			/**
			 * イテレータが示す多角数のindex
			 */
			size_t n_;
		
		};
	} // namespace range_detail

	/**
	 * 多角数列の範囲
	 * 無限数列・有限数列どちらも表現可能
	 * @param Integer 多角数を表現する整数型
	 */
	template<size_t P, typename Integer>
	class polygonal_range
		: public boost::iterator_range< range_detail::polygonal_iterator<P, Integer> >
	{
		/**
		 * イテレータの型
		 */
		typedef range_detail::polygonal_iterator<P, Integer> iterator_t;
		/**
		 * 基底クラス(iterator_range)
		 */
		typedef boost::iterator_range<iterator_t> base_t;
	public:
		/**
		 * 有限多角数列 index指定 [first, last)
		 * @param first 範囲の始め(含む)
		 * @param last  範囲の終わり(含まない)
		 * @return n in [first, last)の多角数列a_n
		 */
		polygonal_range(Integer first, Integer last)
			: base_t(iterator_t(first), iterator_t(last))
		{ }
		/**
		 * 無限多角数列 index指定 [first, Inf)
		 * 単純にalgorithmに渡したりすると無限ループなので注意
		 * @param first 範囲の始め(含む)
		 * @return n in [first, Inf)の多角数列a_n
		 */
		polygonal_range(Integer first)
			: base_t(iterator_t(first), iterator_t())
		{ }
		
	};
	/**
	 * 無限多角数列 index指定 [first, Inf)
	 * 単純にalgorithmに渡したりすると無限ループなので注意
	 *
	 * @param first 範囲の始め(含む)
	 * @return n in [first, Inf)の多角数列を表すrange
	 */
	template<size_t P, typename Integer = int>
	polygonal_range<P, Integer>
	polygonal(Integer first = 0)
	{
		std::cout << P << std::endl;
		BOOST_ASSERT( first >= 0 );
		return polygonal_range<P, Integer>(first);
	}
	/**
	 * 有限多角数列 index指定 [first, last)
	 * @param first 範囲の始め(含む)
	 * @param last  範囲の終わり(含まない)
	 * @return n in [first, last)の多角数列を表すrange
	 */
	template <size_t P, typename Integer = int>
	polygonal_range<P, Integer>
	polygonal(Integer first, Integer last)
	{
		BOOST_ASSERT( first <= last );
		BOOST_ASSERT( first >= 0 );
		return polygonal_range<P, Integer>(first, last);
	}
}
