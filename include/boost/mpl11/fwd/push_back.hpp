/*!
 * @file
 * Forward declares `boost::mpl11::push_back`.
 */

#ifndef BOOST_MPL11_FWD_PUSH_BACK_HPP
#define BOOST_MPL11_FWD_PUSH_BACK_HPP

namespace boost { namespace mpl11 {
    /*!
     * @ingroup container_intrinsics
     *
     * Inserts an element at the end of a `BackExtensibleContainer`.
     */
    template <typename Container, typename Element>
    struct push_back;

    template <typename Container, typename Element>
    using push_back_t = typename push_back<Container, Element>::type;
}} // end namespace boost::mpl11

#endif // !BOOST_MPL11_FWD_PUSH_BACK_HPP
