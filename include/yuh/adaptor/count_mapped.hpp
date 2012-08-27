#pragma once

#include <cmath>

#include <boost/optional.hpp>
#include <boost/assert.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/iterator_range.hpp>


namespace yuh
{
	namespace range_detail
	{
		template <typename Range>
		class count_mapped_range;

		/**
		 * count_map実装部: iterator
		 * @param Range 変換元範囲
		 */
		template <typename Range>
		class count_map_iterator :
			public boost::iterator_facade<
			count_map_iterator<Range>,
			std::pair<
				typename boost::range_value<Range>::type,
				int>,
					boost::bidirectional_traversal_tag,
					std::pair<
						typename boost::range_value<Range>::type,
						int >
			>
		{
			/**
			 * 基底iterator_facade
			 */
			typedef boost::iterator_facade<
				count_map_iterator<Range>,
				std::pair<
					typename boost::range_value<Range>::type,
					int >,
						boost::bidirectional_traversal_tag,
						std::pair<
							typename boost::range_value<Range>::type,
							int >
				> base_t;
			/**
			 * 変換元範囲の値型
			 */
			typedef typename boost::range_value<Range>::type org_value_type;
			/**
			 * 変換元範囲のiterator型
			 */
			typedef typename boost::range_iterator<Range>::type org_iter_type;
			
		public:
			/**
			 * 値型 pair(値, 個数)
			 */
			typedef typename base_t::value_type value_type;
			/**
			 * iterator間距離の型
			 */
			typedef typename base_t::difference_type difference_type;
			/**
			 * 参照剥がしの型
			 * その場で作って返すので非参照
			 */
			typedef typename base_t::reference reference;
			
			/**
			 * ctor
			 * @param it 元範囲の指す位置 変換元範囲の値が連続している箇所の先頭，でなければ挙動がおかしくなる
			 * @param rng 元範囲そのもの
			 */
			count_map_iterator(org_iter_type it, Range& rng)
				: it_(it), first_(std::begin(rng)), last_(std::end(rng))
			{ }
			
		private:
			/**
			 * operator++
			 */
			void increment()
			{
				const auto now = *it_;
				for(; it_!=last_ && *it_==now; ++it_ );
			}
			/**
			 * operator--
			 */
			void decrement()
			{
				if(it_ == first_) return;
				const auto now = *(--it_);
				while(true)
				{
					if(it_ == first_) break;//最初の要素なのでこの前は存在しない
					
					auto pr = std::prev(it_);//前がある
					
					if(*pr != now) break; //前はあったけど違う要素だった
					
					it_ = pr;                  //同じ要素がまだ続いているので先を見なければならない
				}
				
			}
			/**
			 * operator==
			 * @param rhs 比較対象
			 * @return 等値かどうか
			 */ 
			bool equal(const count_map_iterator& rhs) const
			{
				return it_==rhs.it_;
			}
			/**
			 * operator*
			 * @return 参照剥がしの結果 pair(値, 個数)
			 */
			reference dereference() const
			{
				return reference(
					*it_,                         //第一要素は値
					std::distance(                //次の値の先頭との距離=個数
						it_,
						std::find_if(             //今の値と違うものの先頭を探す
							it_, last_,
							[&](org_value_type x) 
							{
								return x != *it_;
							})
						)
					);
									 
			}
			/**
			 * for iterator_facade
			 */
			friend class ::boost::iterator_core_access;
			/**
			 * 現在指している元コンテナの数値の連続している先頭のもの
			 */
			org_iter_type it_;
			/**
			 * 変換Range先頭
			 */
			org_iter_type first_;
			/**
			 * 変換Range末尾
			 */
			org_iter_type last_;
		};
			
		/**
		 * count_map実装部 range
		 * @param Range 変換元範囲
		 */
		template<typename Range>
		class count_mapped_range :
			public boost::iterator_range<count_map_iterator<Range> >
		{
		private:
			/**
			 * 基底iterator_range
			 */
			typedef boost::iterator_range<count_map_iterator<Range> > base_t;
			/**
			 * 変換元範囲の値型
			 */
			typedef typename boost::range_value<Range>::type org_value_type;
			/**
			 * 変換元範囲のiterator型
			 */
			typedef typename boost::range_iterator<Range>::type org_iter_type;
		public:
			/**
			 * iterator型
			 */
			typedef count_map_iterator<Range> iterator;
			/**
			 * ctor
			 * @param rng 変換元範囲
			 */
			count_mapped_range(Range& rng) :
				base_t(
					iterator(std::begin(rng), rng),
					iterator(std::end(rng),  rng))
			{ }
		};

		/**
		 * forwarder
		 */ 
		struct count_map_forwarder {};

		/**
		 * pipe operator
		 * @param Range 
		 * @param r 変換元範囲
		 * @return count_map
		 */
		template<typename Range>
		inline count_mapped_range<Range>
		operator|( Range& r, count_map_forwarder )
		{
			return count_mapped_range<Range>( r );
		}
		/**
		 * pipe operator
		 * @param Range 
		 * @param r 変換元範囲
		 * @return count_map
		 */
		template<typename Range>
		inline count_mapped_range<const Range>
		operator|( const Range& r, count_map_forwarder )
		{
			return count_mapped_range<const Range>( r );
		}
	} // namespace range_detail

	namespace adaptors
	{
		namespace
		{
			using range_detail::count_mapped_range;
			/**
			 * range adaptor
			 * 範囲内の連続した値を(値, 個数)のペアに変換する
			 */
			const auto count_mapped = range_detail::count_map_forwarder();
		}

		/**
		 * function form
		 * @param Range 
		 * @param r 変換元範囲
		 * @return count_map
		 */
		template<typename Range>
		inline count_mapped_range<Range>
		count_map( Range& r )
		{
			return count_mapped_range<Range>( r );
		}
		/**
		 * function form
		 * @param Range 
		 * @param r 変換元範囲
		 * @return count_map
		 */
		template<typename Range>
		inline count_mapped_range<const Range>
		count_map( const Range& r )
		{
			return count_mapped_range<const Range>( r );
		}
	} // namespace adaptors
} // namespace yuh
	
