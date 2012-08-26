#pragma once
#include <type_traits>
#include <boost/range/has_range_iterator.hpp>


namespace yuh
{
	namespace range_detail
	{
		template<typename Range>
		struct range_io;
	}

	namespace
	{
		/**
		 * Never defined: for TMP
		 */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunneeded-internal-declaration"
		extern void * enabler ; //警告されても実体作っちゃうと無駄だし宣言は必要
#pragma clang diagnostic pop
	}

	namespace adaptors
	{
		namespace
		{
			using range_detail::range_io;
		}
		/**
		 * 範囲をrange_ioでくるんで出力可能にする
		 * @param Range 範囲型
		 * @param r 変換元範囲
		 * @return range_io
		 */
		template <
			typename Range, 
			typename std::enable_if<
				std::is_same<
					typename boost::has_range_const_iterator<Range>::type, 
					boost::mpl::true_
					>::value
				> ::type *& = enabler >
		inline range_io<Range> pretty(const Range& r)
		{
			return range_io<Range>(r);
		}
		/**
		 * rangeではないものは素通しする
		 * @param T 値型
		 * @param t Rangeでないオブジェクト
		 * @return tそのもの
		 */
		template<
			typename T, 
			typename std::enable_if<
				std::is_same<
					typename boost::has_range_const_iterator<T>::type, 
					boost::mpl::false_
					>::value
				> ::type *& = enabler >
		inline const T& pretty(const T& t)
		{
			return t;
		}

} // namespace adaptors

	namespace range_detail
	{
		using adaptors::pretty;
		/**
		 * 範囲出力実装部
		 */
		template<typename Range>
		struct range_io
		{
			/**
			 * ctor
			 * @param rng 変換元範囲
			 */
			range_io(const Range& rng):rng_(rng){}

			/**
			 * operator<<から呼び出す
			 * @param os 出力先
			 * @return os
			 */
			template<typename Ch, typename Tr>
			std::basic_ostream<Ch, Tr>& output(std::basic_ostream<Ch, Tr>&os) const
			{
				os << "{ ";
				auto it = std::begin(rng_);
				const auto last = std::end(rng_);
				if (it != last)
				{ 
					os << pretty(*it);
					for( ++it ; it != last ; ++it )
					{
						os << ", " << pretty(*it);
					}
				}
				return os << " }";
			}
		private:
			/**
			 * 対象範囲
			 */
			const Range& rng_;
		};
	
		/**
		 * ストリーム出力
		 * @param os 出力先
		 * @param r 出力する範囲
		 * @return os
		 */
		template<typename Ch, typename Tr, typename Range>
		std::basic_ostream<Ch, Tr> & 
		operator<<(std::basic_ostream<Ch, Tr>& os, const range_io<Range>&r)
		{
			return r.output(os);
		}

		/**
		 * forwarder
		 */
		struct pretty_forwarder 
		{

		};
		
		/**
		 * pipe operator 出力可能なオブジェクトに変換する
		 * @param Range 範囲型
		 * @param r 対象範囲
		 * preturn range_ioオブジェクト
		 */
		template <
			typename Range, 
			typename std::enable_if<
				std::is_same<
					typename boost::has_range_const_iterator<Range>::type, 
					boost::mpl::true_
					>::value
				> ::type *& = enabler >
		inline range_io<Range> 
		operator|(const Range& r, pretty_forwarder)
		{
			return range_io<Range>(r);
		}
	
	} // namespace range_detail

	namespace adaptors
	{
		namespace 
		{
			/**
			 * pipe 
			 * 受け取ったRangeをストリーム出力できる形式に
			 */
			const auto prettied =
				range_detail::pretty_forwarder();
		}
	}
} // namespace yuh
