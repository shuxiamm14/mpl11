/*!
 * @file
 * Defines the @ref Core module.
 *
 *
 * @copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MPL11_CORE_HPP
#define BOOST_MPL11_CORE_HPP

#include <boost/mpl11/fwd/core.hpp>

#include <boost/mpl11/detail/dependent.hpp>
#include <boost/mpl11/functional.hpp>
#include <boost/mpl11/fwd/bool.hpp>
#include <boost/mpl11/fwd/logical.hpp>


namespace boost { namespace mpl11 {
    // undefined
    namespace core_detail {
        template <typename ...xs>
        struct undefined_ {
            static_assert(detail::dependent<xs...>::value(false),
            "Error: `undefined` may not be instantiated.");
        };
    }



    // instantiate
    template <template <typename ...> class Typeclass>
    struct instantiate {
        template <typename ...>
        using with = true_;
    };



    // box
    template <typename x>
    struct box {
        using type = x;
    };



    // datatype
    namespace core_detail {
        template <typename T>
        auto pick_datatype(T*)  -> typename T::mpl_datatype;
        auto pick_datatype(...) -> Foreign;
    }

    template <typename ctor>
    struct datatype {
        using type = decltype(core_detail::pick_datatype((ctor*)nullptr));
    };



    // cast_to
    template <typename To>
    struct cast_to {
        using type = cast_to;

        template <typename x>
        using apply = typename cast<
            typename datatype<x>::type, To
        >::type::template apply<x>;
    };



    // Foreign
    namespace core_detail {
        // Data constructor for `Foreign`. Not sure that should be public.
        // This only preserves type identity. Also; this is not the only
        // data constructor. In fact, any type is a valid data constructor
        // for `Foreign`. I'm not sure I like that.
        template <typename>
        struct foreign {
            using type = foreign;
            using mpl_datatype = Foreign;
        };
    }

    template <typename From>
    struct cast<From, Foreign> {
        using type = cast;

        template <typename x>
        using apply = core_detail::foreign<x>;
    };

    // We can't use `lift<id>` here because `foreign` is not the only
    // data constructor for `Foreign`! Pitfall!
    template <>
    struct cast<Foreign, Foreign> : lift<box> { };
}} // end namespace boost::mpl11


#include <boost/mpl11/bool.hpp>
#include <boost/mpl11/detail/std_is_same.hpp>
#include <boost/mpl11/logical.hpp>


namespace boost { namespace mpl11 {
    // cast
    namespace core_detail {
        template <typename ...datatypes>
        struct invalid_cast {
            static_assert(detail::dependent<datatypes...>::value(false),
            "No cast is provided between the two requested datatypes.");
        };
    }

    // Note that we must not specialize cast<From, From> because that
    // could make the specialization ambiguous when `cast` is defined
    // over parameterized types, say `cast<Vector<T>, Vector<T>>`.
    template <typename From, typename To>
    struct cast
        : if_c<detail::std_is_same<From, To>::value,
            lift<box>,
            core_detail::invalid_cast<From, To>
        >
    { };
}}

#endif // !BOOST_MPL11_CORE_HPP
