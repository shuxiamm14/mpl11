/*!
 * @file
 * Forward declares the @ref Group typeclass.
 *
 *
 * @copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MPL11_FWD_GROUP_HPP
#define BOOST_MPL11_FWD_GROUP_HPP

#include <boost/mpl11/fwd/bool.hpp>


namespace boost { namespace mpl11 {
    /*!
     * @ingroup typeclasses
     * @defgroup Group Group
     *
     * `Monoid` where all objects have an inverse w.r.t. the binary operation.
     *
     * Instances of `Group` must satisfy the following laws:
     *
        @code
            plus x (negate x) == zero
            plus (negate x) x == zero
        @endcode
     *
     * The method names refer to the group of numbers under addition.
     *
     *
     * ### Methods
     * `minus` and `negate`
     *
     * ### Minimal complete definition
     * Either `minus` or `negate`.
     *
     * @{
     */
    template <typename Left, typename Right = Left, typename = true_>
    struct Group;

    //! Equivalent to `plus<x, negate<y>>`.
    template <typename x, typename y>
    struct minus;

    //! Returns the inverse of `x`.
    template <typename x>
    struct negate;
    //! @}
}} // end namespace boost::mpl11

#endif // !BOOST_MPL11_FWD_GROUP_HPP
