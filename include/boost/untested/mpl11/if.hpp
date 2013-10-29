/*!
 * @file
 * Defines `boost::mpl11::if_`.
 */

#ifndef BOOST_MPL11_IF_HPP
#define BOOST_MPL11_IF_HPP

#include <boost/mpl11/fwd/if.hpp>

#include <boost/mpl11/bool.hpp> // for if_c
#include <boost/mpl11/detail/conditional.hpp>
#include <boost/mpl11/detail/optional.hpp>
#include <boost/mpl11/empty_base.hpp>


namespace boost { namespace mpl11 {
namespace if_detail {
    template <typename Cond, typename Then>
    struct branch {
        using condition = Cond;
        using then = Then;
    };

    // We walk the branches until we encounter one whose condition is `true`,
    // and pick its `then`. `evaluate<>` can't possibly be reached because
    // we always provide a `branch<true_, Else>` alternative in the `if_`
    // constructs below.
    template <typename ...Branches>
    struct evaluate;

    template <typename Branch, typename ...Rest>
    struct evaluate<Branch, Rest...>
        : detail::conditional<Branch::condition::type::value,
            identity<typename Branch::then>,
            evaluate<Rest...>
        >::type
    { };

    using detail::optional;
    template <typename ...Branches>
    class accumulate {
        template <typename Cond, typename Then, typename Else = optional>
        struct make_else_if
            : identity<
                evaluate<Branches..., branch<Cond, Then>, branch<true_, Else>>
            >
        { };

        template <typename Cond, typename Then>
        struct make_else_if<Cond, Then>
            : identity<
                accumulate<Branches..., branch<Cond, Then>>
            >
        { };

    public:
        template <typename Cond, typename Then, typename Else = optional>
        using else_if = typename make_else_if<Cond, Then, Else>::type;

        template <bool Cond, typename Then, typename Else = optional>
        using else_if_c = else_if<bool_<Cond>, Then, Else>;

        template <typename Else>
        using else_ = evaluate<Branches..., branch<true_, Else>>;
    };
} // end namespace if_detail

template <typename Cond, typename Then, typename Else>
struct if_ // <Cond, Then, Else>
    : if_detail::accumulate<>::template else_if<Cond, Then, Else>
{ };

template <typename Cond>
struct if_<Cond>
    : if_<Cond, identity<void>, empty_base>::type
{ };

template <typename Cond, typename Then>
struct if_<Cond, Then>
    : if_detail::accumulate<>::template else_if<Cond, Then>
{ };
}} // end namespace boost::mpl11

#endif // !BOOST_MPL11_IF_HPP
