/*!
 * @file
 * Forward declares `boost::mpl11::reverse`.
 */

#ifndef BOOST_MPL11_FWD_REVERSE_HPP
#define BOOST_MPL11_FWD_REVERSE_HPP

namespace boost { namespace mpl11 {
    /*!
     * @ingroup metafunctions
     *
     * Returns the elements of a sequence in reverse order.
     */
    template <typename Sequence>
    struct reverse;

    template <typename Sequence>
    using reverse_t = reverse<Sequence>;
}} // end namespace boost::mpl11

#endif // !BOOST_MPL11_FWD_REVERSE_HPP