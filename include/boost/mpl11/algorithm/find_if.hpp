/*!
 * @file
 * Defines `boost::mpl11::algorithm::find_if`.
 */

#ifndef BOOST_MPL11_ALGORITHM_FIND_IF_HPP
#define BOOST_MPL11_ALGORITHM_FIND_IF_HPP

#include <boost/mpl11/algorithm/find_if_fwd.hpp>
#include <boost/mpl11/dispatch.hpp>
#include <boost/mpl11/functional/apply_wrap.hpp>
#include <boost/mpl11/functional/lambda.hpp>
#include <boost/mpl11/identity.hpp>
#include <boost/mpl11/if.hpp>
#include <boost/mpl11/intrinsic/begin.hpp>
#include <boost/mpl11/intrinsic/deref.hpp>
#include <boost/mpl11/intrinsic/end.hpp>
#include <boost/mpl11/intrinsic/next.hpp>
#include <boost/mpl11/operator/equal_to.hpp>


namespace boost { namespace mpl11 {
namespace find_if_detail {
    template <typename First, typename Last, typename Predicate,
              bool = equal_to<First, Last>::type::value>
    struct find_if_impl
        : identity<Last>
    { };

    template <typename First, typename Last, typename Predicate>
    struct lazy_next_step
        : find_if_impl<
            typename next<First>::type, Last, Predicate
        >
    { };

    template <typename First, typename Last, typename Predicate>
    struct find_if_impl<First, Last, Predicate, false>
        : if_<apply_wrap<Predicate, typename deref<First>::type>,
            identity<First>,
            lazy_next_step<First, Last, Predicate>
        >::type
    { };
} // end namespace find_if_detail

template <typename Sequence, typename Predicate>
struct dispatch<tag::default_<tag::find_if>, Sequence, Predicate>
    : find_if_detail::find_if_impl<
        typename begin<Sequence>::type,
        typename end<Sequence>::type,
        typename lambda<Predicate>::type
    >
{ };
}} // end namespace boost::mpl11

#endif // !BOOST_MPL11_ALGORITHM_FIND_IF_HPP
