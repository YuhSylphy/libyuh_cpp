#pragma once

#include <deque>

#include <boost/range/combine.hpp>
#include <boost/assert.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/algorithm.hpp>

//#include <boost/range/adaptor/regular_extension/filtered.hpp>

namespace yuh
{
	namespace range_detail
	{
		template <typename Range>
		class set_powered_range;

		/**
		 * set_power実装部: iterator
		 * @param Range 変換元範囲
		 */
		template <typename Range>
		class set_power_iterator :
			public boost::iterator_facade<
			set_power_iterator<Range>,
			std::vector<typename boost::range_value<Range>::type >,
			boost::bidirectional_traversal_tag,
			std::vector<typename boost::range_value<Range>::type >
			>
		{
			/**
			 * 基底iterator_facade
			 */
			typedef boost::iterator_facade<
				set_power_iterator<Range>,
				std::vector<typename boost::range_value<Range>::type >,
				boost::bidirectional_traversal_tag,
				std::vector<typename boost::range_value<Range>::type >
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
			 * 値型 元の集合のsubset
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
			set_power_iterator(Range& rng, bool is_first = false)
				: first_(std::begin(rng)), last_(std::end(rng)),
				  flags_(std::distance(first_, last_)+1)
			{
				flags_.back() = !is_first;
			}
		private:
			/**
			 * operator++
			 */
			void increment()
			{
				if(flags_.back()) return;                // 終端フラグが立っている
				const auto it = boost::find(flags_, false);
				
				if(it == std::end(flags_)) return;       //全部1なので終端…って，ここにかかったらおかしいけれど
				
				std::fill(std::begin(flags_), it, false); //この範囲は全部下ろす
				*it = true; //ここだけ立てる(繰り上がり
			}
			/**
			 * operator--
			 */
			void decrement()
			{
				const auto it = 
					boost::find(flags_, true);
				if(it == std::end(flags_)) return;       //全部0なので先頭
				
				std::fill(std::begin(flags_), it, true); //この範囲は全部立てる
				*it = false; //ここだけ下ろす(繰り下がり
			}
			/**
			 * operator==
			 * @param rhs 比較対象
			 * @return 等値かどうか
			 */ 
			bool equal(const set_power_iterator& rhs) const
			{
				return boost::equal(flags_, rhs.flags_);
			}
			/**
			 * operator*
			 * @return 参照剥がしの結果 pair(値, 個数)
			 */
			reference dereference() const
			{
				reference ret;
				ret.reserve(boost::count(flags_, true)); //trueの分だけ領域用意
				
				// boost::copy(
				// 	boost::combine(
				// 		boost::iterator_range(first_, last_),
				// 		flags_) //flagくっつけて
				// 	|+ filter([](boost::tuple<org_value_type, bool>& t) { 
				// 			return boost::get<1>(t); //trueのもの抽出
				// 		})
				// 	|+ transform([](boost::tuple<org_value_type, bool>& t) {
				// 			return boost::get<0>(t); //値だけに変換
				// 		}),
				// 	std::back_inserter(ret)  //追加して行く
				// 	);

				for( const auto& t: boost::combine(
						 std::make_pair(first_,last_), 
						 std::make_pair(std::begin(flags_), std::prev(std::end(flags_))) 
						 ) )
						 
				{   //regular operatorとか使ってみたけどこっちのほうがかなり簡潔である
					const auto& val = boost::get<0>(t);
					const auto flag = boost::get<1>(t);
					
					if(flag) ret.push_back(val);
				}
				
				return ret;
			}
			/**
			 * for iterator_facade
			 */
			friend class ::boost::iterator_core_access;
			/**
			 * 変換Range先頭
			 */
			org_iter_type first_;
			/**
			 * 変換Range末尾
			 */
			org_iter_type last_;
			/**
			 * 現在指している元コンテナの数値の連続している先頭のもの
			 */
			std::deque<bool> flags_;
		};
			
		/**
		 * set_power実装部 range
		 * @param Range 変換元範囲
		 */
		template<typename Range>
		class set_powered_range :
			public boost::iterator_range<set_power_iterator<Range> >
		{
		private:
			/**
			 * 基底iterator_range
			 */
			typedef boost::iterator_range<set_power_iterator<Range> > base_t;
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
			typedef set_power_iterator<Range> iterator;
			/**
			 * ctor
			 * @param rng 変換元範囲
			 */
			set_powered_range(Range& rng) :
				base_t(
					iterator(rng, true),
					iterator(rng))
			{ }
		};

		/**
		 * forwarder
		 */ 
		struct set_power_forwarder {};

		/**
		 * pipe operator
		 * @param Range 
		 * @param r 変換元範囲
		 * @return set_power
		 */
		template<typename Range>
		inline set_powered_range<Range>
		operator|( Range& r, set_power_forwarder )
		{
			return set_powered_range<Range>( r );
		}
		/**
		 * pipe operator
		 * @param Range 
		 * @param r 変換元範囲
		 * @return set_power
		 */
		template<typename Range>
		inline set_powered_range<Range>
		operator|( const Range& r, set_power_forwarder )
		{
			return set_powered_range<Range>( r );
		}
	} // namespace range_detail

	namespace adaptor
	{
		namespace
		{
			using range_detail::set_powered_range;
			/**
			 * range adaptor
			 * 範囲内の連続した値を(値, 個数)のペアに変換する
			 */
			const auto set_powered = range_detail::set_power_forwarder();
		}

		/**
		 * function form
		 * @param Range 
		 * @param r 変換元範囲
		 * @return set_power
		 */
		template<typename Range>
		inline set_powered_range<Range>
		set_power( Range& r )
		{
			return set_powered_range<Range>( r );
		}
		/**
		 * function form
		 * @param Range 
		 * @param r 変換元範囲
		 * @return set_power
		 */
		template<typename Range>
		inline set_powered_range<Range>
		set_power( const Range& r )
		{
			return set_powered_range<Range>( r );
		}
	} // namespace adaptors
} // namespace yuh
	
