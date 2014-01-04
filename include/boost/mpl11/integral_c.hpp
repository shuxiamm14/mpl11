/*!
 * @file
 * Defines `boost::mpl11::integral_c`.
 */

#ifndef BOOST_MPL11_INTEGRAL_C_HPP
#define BOOST_MPL11_INTEGRAL_C_HPP

#include <boost/mpl11/fwd/integral_c.hpp>

#include <boost/mpl11/fwd/and.hpp>
#include <boost/mpl11/fwd/or.hpp>
#include <boost/mpl11/fwd/static_constant.hpp>
#include <boost/mpl11/fwd/tag_of.hpp>


namespace boost { namespace mpl11 {
    template <typename T, T v>
    struct tag_of<integral_c<T, v>> { using type = static_constant_tag; };

    template <typename T, T v>
    struct integral_c {
        using value_type = T;
        static constexpr value_type value = v;
        constexpr operator value_type() const { return value; }
        constexpr value_type operator()() const { return value; }
        using type = integral_c;
    };

    namespace integral_c_detail {
        template <typename ...T>
        struct or_impl : true_ { };

        template <typename ...T>
        struct or_impl<integral_c<T, false>...> : false_ { };

        template <typename ...T>
        struct and_impl : false_ { };

        template <typename ...T>
        struct and_impl<integral_c<T, true>...> : true_ { };
    } // end namespace integral_c_detail

    template <typename T, T ...v>
    struct or_<integral_c<T, v>...>
        : integral_c_detail::or_impl<bool_<static_cast<bool>(v)>...>
    { };

    template <typename T, T ...v>
    struct and_<integral_c<T, v>...>
        : integral_c_detail::and_impl<bool_<static_cast<bool>(v)>...>
    { };
}} // end namespace boost::mpl11

// We need to include it, but we include it once `integral_c` is complete.
#include <boost/mpl11/static_constant.hpp>

#endif // !BOOST_MPL11_INTEGRAL_C_HPP
