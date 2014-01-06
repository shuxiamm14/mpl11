/*!
 * @file
 * Defines `boost::mpl11::Enumerable`.
 */

#ifndef BOOST_MPL11_ENUMERABLE_HPP
#define BOOST_MPL11_ENUMERABLE_HPP

#include <boost/mpl11/fwd/enumerable.hpp>

#include <boost/mpl11/tag_of.hpp>


namespace boost { namespace mpl11 {
    template <typename E>
    struct succ_impl
        : Enumerable<typename tag_of<E>::type>::template succ_impl<E>
    { };

    template <typename E>
    struct pred_impl
        : Enumerable<typename tag_of<E>::type>::template pred_impl<E>
    { };
}} // end namespace boost::mpl11

#endif // !BOOST_MPL11_ENUMERABLE_HPP