#pragma once
#include <type_traits>
#include <iterator>
#include <boost/range/has_range_iterator.hpp>
#include <boost/format.hpp>

#include "../detail/enabler.h"

namespace yuh
{
	namespace range_detail
	{
		template<typename Range>
		struct range_io;
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
			typename Range
#ifndef _MSC_VER
			, typename ::std::enable_if<
				::std::is_same<
					typename boost::has_range_const_iterator<Range>::type, 
					boost::mpl::true_
					>::value
				> ::type *& = enabler 
#endif
			>
		inline range_io<Range> pretty(
			const Range& r,
			const ::std::string& fmt = "%||",
			const ::std::string& opn = "{ ",
			const ::std::string& cls = " }",
			const ::std::string& sep = ", "
#ifdef _MSC_VER
			, typename ::std::enable_if<
				::std::is_same<
					typename boost::has_range_const_iterator<Range>::type, 
					boost::mpl::true_
					>::value
				> ::type *& = enabler
#endif
			)
		{
			return range_io<Range>(r, fmt, opn, cls, sep);
		}
		/**
		 * rangeではないものは素通しする
		 * @param T 値型
		 * @param t Rangeでないオブジェクト
		 * @return tそのもの
		 */
		template<
			typename T
#ifndef _MSC_VER
			, typename ::std::enable_if<
				::std::is_same<
					typename boost::has_range_const_iterator<T>::type, 
					boost::mpl::false_
					>::value
				> ::type *& = enabler 
#endif
		>
		inline const T& pretty(
			const T& t,
			const ::std::string& = "%||",
			const ::std::string& = "{ ",
			const ::std::string& = " }",
			const ::std::string& = ", "
#ifdef _MSC_VER
			, typename ::std::enable_if<
				::std::is_same<
					typename boost::has_range_const_iterator<T>::type, 
					boost::mpl::false_
					>::value
				> ::type *& = enabler 
#endif
			)
		{
			return t;
		}

		template<typename Ch, typename Tr>
		inline ::std::basic_string<Ch, Tr> const& pretty(
			::std::basic_string<Ch, Tr> const& str,
			const ::std::string& = "%||",
			const ::std::string& = "{ ",
			const ::std::string& = " }",
			const ::std::string& = ", "
#ifdef _MSC_VER
			, void* =0
#endif
			)
		{
			return str;
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
			range_io(
				const Range& rng,
				const ::std::string& fmt = "%||",
				const ::std::string& opn = "{ ",
				const ::std::string& cls = " }",
				const ::std::string& sep = ", "
				) : 
				rng_(rng),
				fmt_(fmt), opn_(opn), 
				cls_(cls), sep_(sep)
			{}

			/**
			 * operator<<から呼び出す
			 * @param os 出力先
			 * @return os
			 */
			template<typename Ch, typename Tr>
			::std::basic_ostream<Ch, Tr>& output(
				::std::basic_ostream<Ch, Tr>& os) const
			{
				os << opn_;
				auto it = ::std::begin(rng_);
				const auto last = ::std::end(rng_);
				if (it != last)
				{ 
					os << (boost::format(fmt_) % pretty(*it, fmt_, opn_, cls_, sep_));
					for( ++it ; it != last ; ++it )
					{
						os << sep_ << 
							(boost::format(fmt_) % pretty(*it, fmt_, opn_, cls_, sep_));
					}
				}
				return os << cls_;
			}
		private:
			/**
			 * 対象範囲
			 */
			const Range& rng_;
			/**
			 * boost::format フォーマット文字列
			 */
			::std::string fmt_;
			/**
			 * 開き括弧
			 */
			::std::string opn_;
			/**
			 * 閉じ括弧
			 */
			::std::string cls_;
			/**
			 * 区切り文字列
			 */
			::std::string sep_;
			
		};
	
		/**
		 * ストリーム出力
		 * @param os 出力先
		 * @param r 出力する範囲
		 * @return os
		 */
		template<typename Ch, typename Tr, typename Range>
		::std::basic_ostream<Ch, Tr> & 
		operator<<(::std::basic_ostream<Ch, Tr>& os, const range_io<Range>&r)
		{
			return r.output(os);
		}

		/**
		 * forwarder
		 */
		struct pretty_forwarder 
		{
			/**
			 * ctor
			 * @param fmt フォーマット文字列
			 * @param opn 開き括弧
			 * @param cls 閉じ括弧
			 * @param sep 区切り文字列
			 */
			pretty_forwarder(
				const ::std::string& fmt = "%||",
				const ::std::string& opn = "{ ",
				const ::std::string& cls = " }",
				const ::std::string& sep = ", "
				) : fmt_(fmt), opn_(opn), 
					cls_(cls), sep_(sep) {}

			/**
			 * フォーマット文字列だけ一時設定
			 */
			pretty_forwarder operator()(
				const ::std::string& fmt
				) const 
			{
				return pretty_forwarder(fmt, opn_, cls_, sep_);
			}
			/**
			 * 括弧，区切り文字列を一時設定
			 */
			pretty_forwarder operator()(
				const ::std::string& opn,
				const ::std::string& sep,
				const ::std::string& cls
				) const 
			{
				return pretty_forwarder(fmt_, opn, cls, sep);
			}
			/**
			 * 各パラメータを一時的に設定
			 */
			pretty_forwarder operator()(
				const ::std::string& fmt,
				const ::std::string& opn,
				const ::std::string& sep,
				const ::std::string& cls
				) const
			{
				return pretty_forwarder(fmt, opn, cls, sep);
			}

			/**
			 * フォーマット文字列だけ一時設定
			 */
			pretty_forwarder& format(
				const ::std::string& fmt
				) 
			{
				fmt_ = fmt;
				return *this;
			}
			/**
			 * 括弧，区切り文字列を設定
			 */
			pretty_forwarder& format(
				const ::std::string& opn,
				const ::std::string& sep,
				const ::std::string& cls
				) 
			{
				opn_ = opn;
				cls_ = cls;
				sep_ = sep;
				return *this;
			}
			/**
			 * 各パラメータを設定
			 */
			pretty_forwarder& format(
				const ::std::string& fmt,
				const ::std::string& opn,
				const ::std::string& sep,
				const ::std::string& cls
				) 
			{
				fmt_ = fmt;
				opn_ = opn;
				cls_ = cls;
				sep_ = sep;
				return *this;
			}

			const ::std::string getFmt() const { return fmt_; }
			const ::std::string getOpn() const { return opn_; }
			const ::std::string getCls() const { return cls_; }
			const ::std::string getSep() const { return sep_; }
		private:
			/**
			 * boost::format フォーマット文字列
			 */
			::std::string fmt_;
			/**
			 * 開き括弧
			 */
			::std::string opn_;
			/**
			 * 閉じ括弧
			 */
			::std::string cls_;
			/**
			 * 区切り文字列
			 */
			::std::string sep_;
		};
		
		/**
		 * pipe operator 出力可能なオブジェクトに変換する
		 * @param Range 範囲型
		 * @param r 対象範囲
		 * preturn range_ioオブジェクト
		 */
		template <
			typename Range
#ifndef _MSC_VER
			, typename ::std::enable_if<
				::std::is_same<
					typename boost::has_range_const_iterator<Range>::type, 
					boost::mpl::true_
					>::value
				> ::type *& = enabler 
#endif
			>
		inline range_io<Range> 
		operator|(const Range& r, pretty_forwarder f)
		{
			return range_io<Range>(
				r,
				f.getFmt(), f.getOpn(), f.getCls(), f.getSep());
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
			auto prettied =
				range_detail::pretty_forwarder();
		}
	}
} // namespace yuh
