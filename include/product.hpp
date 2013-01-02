/** http://d.hatena.ne.jp/iorate/20110220/1298200968 */

#ifndef BOOST_PP_IS_ITERATING

#ifndef IORATE_PRODUCT_HPP_INCLUDED
#define IORATE_PRODUCT_HPP_INCLUDED

#include <boost/lambda/lambda.hpp>
#include <boost/iterator/iterator_categories.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/min_element.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/preprocessor/arithmetic.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/type_traits/is_convertible.hpp>

#ifndef IORATE_PRODUCT_MAX_ARITY
#define IORATE_PRODUCT_MAX_ARITY 10
#endif

namespace iorate {

#define BOOST_PP_ITERATION_LIMITS (2, IORATE_PRODUCT_MAX_ARITY)
#define BOOST_PP_FILENAME_1 "product.hpp"
#include BOOST_PP_ITERATE()

}

#endif // IORATE_PRODUCT_HPP_INCLUDED

#else // BOOST_PP_IS_ITERATING

#define n BOOST_PP_ITERATION()

namespace detail {

#define PRODUCT_iterator BOOST_PP_CAT(product_iterator, n)

template <BOOST_PP_ENUM_PARAMS(n, class Iterator)>
struct PRODUCT_iterator;

#define PRODUCT_trait(z, m, trait) \
    typename boost::BOOST_PP_CAT(iterator_, trait)< \
        BOOST_PP_CAT(Iterator, m) \
    >::type

template <BOOST_PP_ENUM_PARAMS(n, class Iterator)>
struct BOOST_PP_CAT(product_iterator_super, n)
{
    typedef boost::iterator_facade<
        PRODUCT_iterator<BOOST_PP_ENUM_PARAMS(n, Iterator)>,
        boost::tuple<BOOST_PP_ENUM(n, PRODUCT_trait, value)>,
        typename boost::mpl::deref<
            typename boost::mpl::min_element<
                boost::mpl::vector<BOOST_PP_ENUM(n, PRODUCT_trait, traversal)>,
                boost::is_convertible<boost::mpl::_2, boost::mpl::_1>
            >::type
        >::type,
        boost::tuple<BOOST_PP_ENUM(n, PRODUCT_trait, value)>
    > type;
};

#undef PRODUCT_trait

template <BOOST_PP_ENUM_PARAMS(n, class Iterator)>
struct PRODUCT_iterator
  : BOOST_PP_CAT(product_iterator_super, n)<
        BOOST_PP_ENUM_PARAMS(n, Iterator)
    >::type
{
private:
    typedef typename BOOST_PP_CAT(product_iterator_super, n)<
        BOOST_PP_ENUM_PARAMS(n, Iterator)
    >::type super_t;

    typedef typename super_t::reference ref_t;
    typedef typename super_t::difference_type diff_t;

public:
    PRODUCT_iterator()
    {}

#define PRODUCT_parameter(z, m, unused) \
    BOOST_PP_CAT(Iterator, m) BOOST_PP_CAT(first, m), \
    BOOST_PP_CAT(Iterator, m) BOOST_PP_CAT(last, m), \
    BOOST_PP_CAT(Iterator, m) BOOST_PP_CAT(it, m)

#define PRODUCT_initializer(z, m, unused) \
    BOOST_PP_CAT(m_first, m)(BOOST_PP_CAT(first, m)), \
    BOOST_PP_CAT(m_last, m)(BOOST_PP_CAT(last, m)), \
    BOOST_PP_CAT(m_it, m)(BOOST_PP_CAT(it, m))

    PRODUCT_iterator(BOOST_PP_ENUM(n, PRODUCT_parameter, ~))
      : BOOST_PP_ENUM(n, PRODUCT_initializer, ~)
    {}

#undef PRODUCT_initializer
#undef PRODUCT_parameter

private:
#define PRODUCT_member(z, m, unused) \
    BOOST_PP_CAT(Iterator, m) \
    BOOST_PP_CAT(m_first, m), BOOST_PP_CAT(m_last, m), BOOST_PP_CAT(m_it, m);

    BOOST_PP_REPEAT(n, PRODUCT_member, ~)
        
#undef PRODUCT_member

#define PRODUCT_short_size(z, l, unused) \
    * (BOOST_PP_CAT(m_last, l) - BOOST_PP_CAT(m_first, l))

#define PRODUCT_size(m) \
    (1 BOOST_PP_REPEAT_FROM_TO(BOOST_PP_INC(m), n, PRODUCT_short_size, ~))

    diff_t get_position() const
    {
#define PRODUCT_get(z, m, unused) \
        + (BOOST_PP_CAT(m_it, m) - BOOST_PP_CAT(m_first, m)) * PRODUCT_size(m)

        return 0 BOOST_PP_REPEAT(n, PRODUCT_get, ~);

#undef PRODUCT_get
    }

    void set_position(diff_t position)
    {
#define PRODUCT_set(z, m, unused) \
        BOOST_PP_CAT(m_it, m) += position / PRODUCT_size(m); \
        position %= PRODUCT_size(m);

        BOOST_PP_REPEAT(n, PRODUCT_set, ~)

#undef PRODUCT_set
    }

#undef PRODUCT_size
#undef PRODUCT_short_size

    friend class boost::iterator_core_access;

    ref_t dereference() const
    {
        return ref_t(BOOST_PP_ENUM_PARAMS(n, *m_it));
    }

    bool equal(PRODUCT_iterator const &other) const
    {
#define PRODUCT_equal(z, m, unused) \
    && BOOST_PP_CAT(m_it, m) == BOOST_PP_CAT(other.m_it, m)

        return true BOOST_PP_REPEAT(n, PRODUCT_equal, ~);

#undef PRODUCT_equal
    }

    void increment()
    {
#define PRODUCT_increment(z, m, unused) \
    if (++BOOST_PP_CAT(m_it, BOOST_PP_SUB(n, m)) == \
        BOOST_PP_CAT(m_last, BOOST_PP_SUB(n, m))) {

#define PRODUCT_assign(z, m, unused) \
        BOOST_PP_CAT(m_it, m) = BOOST_PP_CAT(m_first, m); \
    }

        BOOST_PP_REPEAT_FROM_TO(1, n, PRODUCT_increment, ~)
            ++m_it0;
        BOOST_PP_REPEAT_FROM_TO(1, n, PRODUCT_assign, ~)

#undef PRODUCT_assign
#undef PRODUCT_increment
    }

    void decrement()
    {
#define PRODUCT_equal(z, m, unused) \
    if (BOOST_PP_CAT(m_it, BOOST_PP_SUB(n, m)) == \
        BOOST_PP_CAT(m_first, BOOST_PP_SUB(n, m))) {

#define PRODUCT_decrement(z, m, unused) \
        BOOST_PP_CAT(m_it, m) = BOOST_PP_CAT(m_last, m); \
    }\
    --BOOST_PP_CAT(m_it, m);

        BOOST_PP_REPEAT_FROM_TO(1, n, PRODUCT_equal, ~)
            --m_it0;
        BOOST_PP_REPEAT_FROM_TO(1, n, PRODUCT_decrement, ~)

#undef PRODUCT_decrement
#undef PRODUCT_equal
    }

    void advance(diff_t diff)
    {
        set_position(get_position() + diff);
    }

    diff_t distance_to(PRODUCT_iterator const &other) const
    {
        return other.get_position() - get_position();
    }
};

} // namespace detail

namespace product_detail {

template <BOOST_PP_ENUM_PARAMS(n, class Range)>
struct BOOST_PP_CAT(base, n)
{
#define PRODUCT_const_iterator(z, m, unused) \
    typename boost::range_const_iterator<BOOST_PP_CAT(Range, m)>::type

    typedef detail::PRODUCT_iterator<
        BOOST_PP_ENUM(n, PRODUCT_const_iterator, ~)
    > iter_t;

#undef PRODUCT_const_iterator

    typedef boost::iterator_range<iter_t> result_type;

    result_type
    operator()(BOOST_PP_ENUM_BINARY_PARAMS(n, Range, const &rng)) const
    {
#define PRODUCT_parameter(z, m, unused) \
        boost::begin(BOOST_PP_CAT(rng, m)), boost::end(BOOST_PP_CAT(rng, m))

        return aux(BOOST_PP_ENUM(n, PRODUCT_parameter, ~));

#undef PRODUCT_parameter
    }

#define PRODUCT_parameter(z, m, unused) \
    BOOST_PP_CAT(Iterator, m) BOOST_PP_CAT(first, m), \
    BOOST_PP_CAT(Iterator, m) BOOST_PP_CAT(last, m)

    template <BOOST_PP_ENUM_PARAMS(n, class Iterator)>
    result_type aux(BOOST_PP_ENUM(n, PRODUCT_parameter, ~)) const
    {

#undef PRODUCT_parameter

#define PRODUCT_parameter(z, m, unused) \
        BOOST_PP_CAT(first, m), BOOST_PP_CAT(last, m), BOOST_PP_CAT(first, m)

        return result_type(
            iter_t(BOOST_PP_ENUM(n, PRODUCT_parameter, ~)),
            iter_t(
                first0, last0, last0,
                BOOST_PP_ENUM_SHIFTED(n, PRODUCT_parameter, ~)
            )
        );

#undef PRODUCT_parameter
    }
};

#define PRODUCT_adaptor BOOST_PP_CAT(product_adaptor, n)

template <BOOST_PP_ENUM_SHIFTED_PARAMS(n, class Range)>
struct PRODUCT_adaptor
{
#define PRODUCT_initializer(z, m, unused) \
    BOOST_PP_CAT(m_rng, m)(BOOST_PP_CAT(rng, m))

    explicit PRODUCT_adaptor(
        BOOST_PP_ENUM_SHIFTED_BINARY_PARAMS(n, Range, const &rng)
    ) : BOOST_PP_ENUM_SHIFTED(n, PRODUCT_initializer, ~)
    {}

#undef PRODUCT_initializer

#define PRODUCT_member(z, m, unused) \
    BOOST_PP_CAT(Range, m) const &BOOST_PP_CAT(m_rng, m);

    BOOST_PP_REPEAT_FROM_TO(1, n, PRODUCT_member, ~)

#undef PRODUCT_member
};

template <BOOST_PP_ENUM_PARAMS(n, class Range)>
typename
BOOST_PP_CAT(base, n)<BOOST_PP_ENUM_PARAMS(n, Range)>::result_type
operator|(
    Range0 const &rng0,
    PRODUCT_adaptor<BOOST_PP_ENUM_SHIFTED_PARAMS(n, Range)> adaptor
)
{
    return BOOST_PP_CAT(base, n)<BOOST_PP_ENUM_PARAMS(n, Range)>()(
        rng0,
        BOOST_PP_ENUM_SHIFTED_PARAMS(n, adaptor.m_rng)
    );
}

} // namespace product_detail

template <BOOST_PP_ENUM_SHIFTED_PARAMS(n, class Range)>
product_detail::PRODUCT_adaptor<BOOST_PP_ENUM_SHIFTED_PARAMS(n, Range)>
product(BOOST_PP_ENUM_SHIFTED_BINARY_PARAMS(n, Range, const &rng))
{
    return 
    product_detail::PRODUCT_adaptor<BOOST_PP_ENUM_SHIFTED_PARAMS(n, Range)>(
        BOOST_PP_ENUM_SHIFTED_PARAMS(n, rng)
    );
}

#undef PRODUCT_adaptor
#undef PRODUCT_iterator
#undef n

#endif // BOOST_PP_IS_ITERATING