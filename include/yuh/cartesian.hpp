#pragma once
#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/reference.hpp>
#include <tuple>

namespace yuh
{
	namespace detail
	{
		template<typename...Args>
		struct org_args{};
			
		/*
		 * range列をvalue_type列に変換
		 */
		template<typename...Args>
		struct value_tuple
		{
		 	typedef typename value_tuple<org_args<Args...> >::type type;
		};
		template<typename T, typename... Org, typename... Args>
		struct value_tuple<org_args<T, Org...>, Args...>
		{
			typedef typename value_tuple<
				org_args<Org...>, 
				Args...,
				typename boost::range_value<T>::type
				>::type type;
	};
	template<typename... Args>
	struct value_tuple<org_args<>, Args...>
	{
		typedef std::tuple<Args...> type;
	};
		
	/*
	 * range列をiterator_type列に変換
	 */
	template<typename...Args>
	struct iterator_tuple
	{
		typedef typename iterator_tuple<org_args<Args...> >::type type;
	};
	template<typename T, typename... Org, typename... Args>
	struct iterator_tuple<org_args<T, Org...>, Args...>
	{
		typedef typename iterator_tuple<
			org_args<Org...>, 
			Args...,
			typename boost::range_iterator< T >::type
			>::type type;
	};
		template<typename... Args>
		struct iterator_tuple<org_args<>, Args...>
		{
			typedef std::tuple<Args...> type;
		};
		
/*
 * range列をrange&列に変換
 */
		template<typename...Args>
		struct reference_tuple
		{
			typedef typename reference_tuple<org_args<Args...> >::type type;
		};
		template<typename T, typename... Org, typename... Args>
		struct reference_tuple<org_args<T, Org...>, Args...>
		{
			typedef typename reference_tuple<
				org_args<Org...>, 
				Args...,
				T&
				>::type type;
		};
		template<typename... Args>
		struct reference_tuple<org_args<>, Args...>
		{
			typedef std::tuple<Args...> type;
		};


/*
 * 直積をiterationする
 * value_typeはstd::tuple<>
 */
		template<typename... Args>
		struct cartesian_iterator
			: boost::iterator_facade <
			cartesian_iterator<Args...>,
			typename value_tuple<Args...>::type,
			boost::forward_traversal_tag,
			typename value_tuple<Args...>::type
			
			>
		{
			cartesian_iterator(Args&... args)
				: rng_(args...),
				  it_()
			{ begin<0>(); }

			
			cartesian_iterator make_end_iterator()
			{
				return cartesian_iterator(*this).end<0>();
			}
private:
	friend class boost::iterator_core_access;
	typename reference_tuple<Args...>::type rng_;
	typename iterator_tuple<Args...>::type it_;
			
			template<size_t Index>
			typename std::enable_if<
				sizeof...(Args) == Index,
				cartesian_iterator&  >::type
			end()
				{
					return *this;
				}
		
			template<size_t Index>
			typename std::enable_if<
				(sizeof...(Args) > Index), // <
				cartesian_iterator&  >::type
				end()
				{
					std::get<Index>(it_) = std::end(std::get<Index>(rng_));
					return end<Index+1>();
				}
			template<size_t Index>
			typename std::enable_if<
				sizeof...(Args) == Index,
				cartesian_iterator&  >::type
			begin()
				{
					return *this;
				}
		
			template<size_t Index>
			typename std::enable_if<
				(sizeof...(Args) > Index), // <
				cartesian_iterator&  >::type
				begin()
				{
					std::get<Index>(it_) = std::begin(std::get<Index>(rng_));
					return begin<Index+1>();
				}



		template<size_t Index>
		typename std::enable_if<
			(Index < sizeof...(Args)),
			 bool>::type 
			inc()
{
	auto& i = ++std::get<Index>(it_);
	auto& r = std::get<Index>(rng_);
	bool ret = true;
	if(i == std::end(r))
	{
		if((ret = inc<Index+1>()))
		{
			i = std::begin(r);
		}
	}
	return ret; 
}
template<size_t Index>
typename std::enable_if<
	(Index == sizeof...(Args)),
	bool>::type 
inc()
{
	return false;
}		
		
void increment() 
{
	inc<0>();
}
template<typename...Vals>
typename std::enable_if<
	(sizeof...(Vals) < sizeof...(Args)),
		typename value_tuple<Args...>::type >::type
deref(Vals&... args) const
{
	return deref(args..., *std::get<sizeof...(Vals)>(it_));
}
	template<typename...Vals>
	typename std::enable_if<
		sizeof...(Args) == sizeof...(Vals),
		typename value_tuple<Args...>::type >::type
	deref(Vals&... args)const
	{
		return std::make_tuple(args...);
	}
		
typename value_tuple<Args...>::type dereference() const 
{
	return deref<>();
}
bool equal(cartesian_iterator const& other) const
{
	return it_ == other.it_;
}
};
	
// tamplate<typename... Args, typename... It>
// typename std::enable_if<
// 	sizeof...(Args) == sizeof...(It),
// 	cartesian_iterator<Args...> >
// make_end_iterator_impl(
// 	std::tuple<Args...>& t,
// 	It... it)
// {
// 	return std::make_tuple(it...);
// }
// tamplate<typename... Args, typename... It>
// typename std::enable_if<
// 	sizeof...(Args) > sizeof...(It),
// 	cartesian_iterator<Args...> >
// make_end_iterator_impl(
// 	std::tuple<Args...>& t,
// 	It... it)
// {
// 	return make_end_iterator_impl(
// 		t, it, 
// 		std::end(std::get<sizeof...(It)>(t));
// }

// // std::tupleにstd::end(rng)を詰めて返したい
// template<typename... Args>
// cartesian_iterator<Args...> make_end_iterator(Args&... args)
// {
// 	return make_end_iterator_impl(std::make_tuple(args...));
// }
	
}

template<typename... Args>
using cartesian_range =
	boost::iterator_range<detail::cartesian_iterator<Args...> >;
	

/* 
 * 複数の範囲からその直積を作る
 * @param args 任意個のrange
 * @return 直積を表すrange
 */
template<typename... Args>
cartesian_range<Args...> 
cartesian(Args&... args)
{
	detail::cartesian_iterator<Args...> it(args...) ;
	return cartesian_range<Args...>(
		it,
		it.make_end_iterator()
		);
}
}
