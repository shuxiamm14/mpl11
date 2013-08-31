/*!
 * @file
 * Defines `boost::mpl11::intrinsic::greater_equal`.
 */

#ifndef BOOST_MPL11_INTRINSIC_GREATER_EQUAL_HPP
#define BOOST_MPL11_INTRINSIC_GREATER_EQUAL_HPP

#ifdef BOOST_MPL11_DOXYGEN_INVOKED

namespace boost { namespace mpl11 { namespace intrinsic {
    /*!
     * @ingroup comparison_intrinsic
     *
     * Returns whether `T1 >= T2 >= ...Tn`.
     */
    template <typename T1, typename T2, typename ...Tn>
    struct greater_equal { };
}}} // end namespace boost::mpl11::intrinsic

#else // Doxygen not invoked

#   define BOOST_MPL11_OPERATOR_NAME greater_equal
#   define BOOST_MPL11_OPERATOR >=
#   include <boost/mpl11/detail/generate_comparison_intrinsic.hpp>

#endif // BOOST_MPL11_DOXYGEN_INVOKED

#endif // !BOOST_MPL11_INTRINSIC_GREATER_EQUAL_HPP
