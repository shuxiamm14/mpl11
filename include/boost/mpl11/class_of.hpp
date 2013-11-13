/*!
 * @file
 * Defines `boost::mpl11::class_of`.
 */

#ifndef BOOST_MPL11_CLASS_OF_HPP
#define BOOST_MPL11_CLASS_OF_HPP

#include <boost/mpl11/fwd/class_of.hpp>

#include <boost/mpl11/type.hpp>


namespace boost { namespace mpl11 {
    namespace class_of_detail {
        template <typename T>
        auto pick(void*) -> typename T::mpl_class;

        template <typename T>
        auto pick(...) -> Type;
    } // end namespace class_of_detail

    template <typename T>
    struct class_of {
        using type = decltype(class_of_detail::pick<T>(nullptr));
    };
}} // end namespace boost::mpl11

#endif // !BOOST_MPL11_CLASS_OF_HPP