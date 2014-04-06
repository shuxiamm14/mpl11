/*!
 * @file
 * Defines the @ref Iterable typeclass.
 *
 *
 * @copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MPL11_ITERABLE_HPP
#define BOOST_MPL11_ITERABLE_HPP

#include <boost/mpl11/fwd/iterable.hpp>

#include <boost/mpl11/bool.hpp>
#include <boost/mpl11/comparable.hpp>
#include <boost/mpl11/core.hpp>
#include <boost/mpl11/detail/assertions.hpp>
#include <boost/mpl11/detail/config.hpp>
#include <boost/mpl11/detail/std_size_t.hpp>
#include <boost/mpl11/foldable.hpp>
#include <boost/mpl11/functional.hpp> //
#include <boost/mpl11/integer.hpp>    // required by fwd/iterable.hpp
#include <boost/mpl11/logical.hpp>    //
#include <boost/mpl11/orderable.hpp>


namespace boost { namespace mpl11 {
namespace iterable_detail {
    template <detail::std_size_t n, typename iter,
        bool = n == 0 || is_empty<iter>::value>
    struct drop_impl;

    template <detail::std_size_t n, typename iter>
    struct drop_impl<n, iter, true> : iter { };

    template <detail::std_size_t n, typename iter>
    struct drop_impl<n, iter, false>
        : drop_impl<n-1, tail<iter>>
    { };


    template <typename pred, typename iter, bool = is_empty<iter>::value>
    struct drop_while_impl1;

    template <typename pred, typename iter,
        bool = pred::type::template apply<head<iter>>::type::value>
    struct drop_while_impl2;

    template <typename pred, typename iter>
    struct drop_while_impl1<pred, iter, false>
        : drop_while_impl2<pred, iter>
    { };

    template <typename pred, typename iter>
    struct drop_while_impl1<pred, iter, true> : iter { };

    template <typename pred, typename iter>
    struct drop_while_impl2<pred, iter, true>
        : drop_while_impl1<pred, tail<iter>>
    { };

    template <typename pred, typename iter>
    struct drop_while_impl2<pred, iter, false> : iter { };
} // end namespace iterable_detail

template <typename pred, typename iter>
struct drop_while : iterable_detail::drop_while_impl1<pred, iter> { };

#if 0
    template <typename N, typename Iter, typename>
    struct drop_impl
        : if_<or_<bool_<N::type::value == 0>, is_empty<Iter>>,
            Iter,
            drop<pred<N>, tail<Iter>>
        >
    { };

    template <typename N1, typename N2, typename Iter>
    struct drop_impl<N1, drop<N2, Iter>> : drop<plus<N1, N2>, Iter> { };

    template <typename N, typename Iter>
    struct head_impl<drop<N, Iter>> : at<Iter, N> { };

    template <typename N, typename Iter>
    struct is_empty_impl<drop<N, Iter>
        , bool_<sequence_traits<typename Iter::type>::has_O1_length>
    >
        : bool_<(N::type::value >= length<Iter>::value)>
    { };

    template <typename N, typename Iter>
    struct length_impl<drop<N, Iter>>
        : size_t<(
            N::type::value < length<Iter>::value
                ? length<Iter>::value - N::type::value
                : 0
        )>
    { };

    template <typename N, typename Iter>
    struct last_impl<drop<N, Iter>> : last<Iter> { };

    template <typename N, typename Iter, typename Index>
    struct at_impl<drop<N, Iter>, Index> : at<Iter, plus<N, Index>> { };

    template <typename Pred, typename Iter>
    struct is_empty_impl<drop_until<Pred, Iter>> : none<Pred, Iter> { };

    template <typename Pred, typename Iter>
    struct is_empty_impl<drop_while<Pred, Iter>> : all<Pred, Iter> { };
#endif

namespace iterable_detail {
    template <typename iter>
    struct head_checks {
        static_assert(!is_empty<iter>::value,
        "Invalid usage of `head` on an empty iterable.");
    };

    template <typename iter>
    struct tail_checks {
        static_assert(!is_empty<iter>::value,
        "Invalid usage of `tail` on an empty iterable.");
    };

    template <typename iter>
    struct last_checks {
        static_assert(!is_empty<iter>::value,
        "Invalid usage of `last` on an empty iterable.");
    };

    template <typename index, typename iter>
    struct at_checks {
        static_assert(index::type::value >= 0,
        "Invalid usage of `at` with a negative index.");

        /*
        using Length = if_c<sequence_traits<typename Iter::type>::is_finite,
            length<Iter>,
            size_t<index::type::value + 1>
        >;

        static_assert(index::type::value < Length::value,
        "Invalid usage of `at` with an out-of-bounds index.");
        */
    };

    template <typename n, typename iter>
    struct drop_checks {
        static_assert(n::type::value >= 0,
        "Invalid usage of `drop`: "
        "The number of elements to drop must be non-negative.");
    };
}

template <typename iter>
struct head :
    BOOST_MPL11_IF_ASSERTIONS(iterable_detail::head_checks<iter>,)
    Iterable<typename datatype<typename iter::type>::type>::
    template head_impl<typename iter::type>
{ };

template <typename iter>
struct tail :
    BOOST_MPL11_IF_ASSERTIONS(iterable_detail::tail_checks<iter>,)
    Iterable<typename datatype<typename iter::type>::type>::
    template tail_impl<typename iter::type>
{ };

template <typename iter>
struct last :
    BOOST_MPL11_IF_ASSERTIONS(iterable_detail::last_checks<iter>,)
    Iterable<typename datatype<typename iter::type>::type>::
    template last_impl<typename iter::type>
{ };

template <typename index, typename iter>
struct at :
    BOOST_MPL11_IF_ASSERTIONS(iterable_detail::at_checks<index, iter>,)
    Iterable<typename datatype<typename iter::type>::type>::
    template at_impl<index, typename iter::type>
{ };

template <typename n, typename iter>
struct drop :
    BOOST_MPL11_IF_ASSERTIONS(iterable_detail::drop_checks<n, iter>,)
    iterable_detail::drop_impl<n::type::value, iter>
{ };

template <typename iter>
struct is_empty :
    Iterable<typename datatype<typename iter::type>::type>::
    template is_empty_impl<typename iter::type>
{ };

template <typename iter>
struct length :
    Iterable<typename datatype<typename iter::type>::type>::
    template length_impl<typename iter::type>
{ };

template <typename Datatype, typename>
struct Iterable : false_ { };

template <>
struct instantiate<Iterable> {
    template <typename Datatype>
    struct with : true_ {
        template <typename xs, bool = is_empty<tail<box<xs>>>::value>
        struct last_impl : head<box<xs>> { };

        template <typename xs>
        struct last_impl<xs, false>
            : last_impl<typename tail<box<xs>>::type>
        { };


        template <typename index, typename xs, bool = index::type::value == 0>
        struct at_impl
            : head<box<xs>>
        { };

        template <typename index, typename xs>
        struct at_impl<index, xs, false>
            : at_impl<
                size_t<index::type::value - 1>,
                typename tail<box<xs>>::type
            >
        { };


        template <typename xs, detail::std_size_t acc = 0,
            bool = is_empty<box<xs>>::value>
        struct length_impl : size_t<acc> { };

        template <typename xs, detail::std_size_t acc>
        struct length_impl<xs, acc, false>
            : length_impl<typename tail<box<xs>>::type, acc+1>
        { };
    };
};

template <typename X, typename Y>
struct Orderable<X, Y, bool_<Iterable<X>::value && Iterable<Y>::value>>
    : instantiate<Orderable>::template with<X, Y>
{
    // xs is shorter than ys
    template <typename xs, typename ys,
        bool xs_done = is_empty<box<xs>>::value,
        bool ys_done = is_empty<box<ys>>::value
    >
    struct less_impl
        : bool_<xs_done && !ys_done>
    { };

    // (head(xs) < head(ys)) or (head(xs) == head(ys) and compare the rest)
    template <typename xs, typename ys>
    struct less_impl<xs, ys, false, false>
        : or_<
            less<head<box<xs>>, head<box<ys>>>,
            and_<
                not_<less<head<box<ys>>, head<box<xs>>>>,
                less_impl<
                    typename tail<box<xs>>::type,
                    typename tail<box<ys>>::type
                >
            >
        >
    { };
};

template <typename X, typename Y>
struct Comparable<X, Y, bool_<Iterable<X>::value && Iterable<Y>::value>>
    : instantiate<Comparable>::template with<X, Y>
{
    template <typename xs, typename ys,
        bool xs_done = is_empty<box<xs>>::value,
        bool ys_done = is_empty<box<ys>>::value>
    struct equal_impl
        : bool_<xs_done && ys_done>
    { };

    template <typename xs, typename ys>
    struct equal_impl<xs, ys, false, false>
        : and_<
            equal<head<box<xs>>, head<box<ys>>>,
            equal_impl<
                typename tail<box<xs>>::type,
                typename tail<box<ys>>::type
            >
        >
    { };
};

template <typename Datatype>
struct Foldable<Datatype, typename Iterable<Datatype>::type>
    : instantiate<Foldable>::template with<Datatype>
{
    template <typename f, typename state, typename iter,
        bool = is_empty<box<iter>>::value>
    struct foldl_impl
        : state
    { };

    template <typename f, typename state, typename iter>
    struct foldl_impl<f, state, iter, false>
        : foldl_impl<
            f,
            typename f::type::template apply<state, head<box<iter>>>,
            typename tail<box<iter>>::type
        >
    { };


    template <typename f, typename iter>
    using foldl1_impl = foldl_impl<
        f, head<box<iter>>, typename tail<box<iter>>::type
    >;


    template <typename f, typename state, typename iter,
        bool = is_empty<box<iter>>::value>
    struct foldr_impl
        : state
    { };

    template <typename f, typename state, typename iter>
    struct foldr_impl<f, state, iter, false>
        : f::type::template apply<
            head<box<iter>>,
            foldr_impl<f, state, typename tail<box<iter>>::type>
        >
    { };


    template <typename f, typename iter,
        bool = is_empty<tail<box<iter>>>::value>
    struct foldr1_impl
        : head<box<iter>>
    { };

    template <typename f, typename iter>
    struct foldr1_impl<f, iter, false>
        : f::type::template apply<
            head<box<iter>>,
            foldr1_impl<f, typename tail<box<iter>>::type>
        >
    { };
};
}} // end namespace boost::mpl11

#endif // !BOOST_MPL11_ITERABLE_HPP
