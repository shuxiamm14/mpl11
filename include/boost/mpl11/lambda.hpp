/*!
 * @file
 * Defines `boost::mpl11::lambda`.
 */

#ifndef BOOST_MPL11_LAMBDA_HPP
#define BOOST_MPL11_LAMBDA_HPP

#include <boost/mpl11/apply_wrap.hpp>
#include <boost/mpl11/as_placeholder.hpp>
#include <boost/mpl11/bind.hpp>
#include <boost/mpl11/identity.hpp>
#include <boost/mpl11/if.hpp>
#include <boost/mpl11/is_placeholder.hpp>
#include <boost/mpl11/is_placeholder_expression.hpp>
#include <boost/mpl11/quote.hpp>


namespace boost { namespace mpl11 {
namespace lambda_detail {
    template <typename F>
    struct bind_nested_placeholder_expr;

    template <template <typename ...> class F, typename ...T>
    struct bind_nested_placeholder_expr<F<T...>>
        : identity<
            // Using `as_placeholder` will cause this nested `bind` to be
            // applied when the outer `bind` is applied.
            as_placeholder<
                bind<quote<F>,
                    typename if_<is_placeholder<T>,
                        identity<T>
                    >::template else_if<is_placeholder_expression<T>,
                        bind_nested_placeholder_expr<T>
                    >::template else_<
                        identity<T>
                    >::type::type...
                >
            >
        >
    { };

    template <typename F, bool = is_placeholder<F>::type::value>
    struct anonymous;

    // We have a placeholder expression that is not a placeholder.
    template <typename F>
    struct anonymous<F, false> {
        struct type {
            template <typename ...Args>
            struct apply
                : apply_wrap<
                    typename bind_nested_placeholder_expr<F>::type, Args...
                >
            { };
        };
    };

    // We have a placeholder.
    template <typename F>
    struct anonymous<F, true> {
        struct type {
            template <typename ...Args>
            struct apply
                : apply_wrap<F, Args...>
            { };
        };
    };
} // end namespace lambda_detail

/*!
 * Transforms a `LambdaExpression` into its corresponding `MetafunctionClass`.
 *
 * If `T` is not a `PlaceholderExpression`, returns `T` unchanged.
 *
 * Otherwise, if `T` is a `Placeholder`, returns a non-template
 * `MetafunctionClass` without base class and equivalent to `T`
 * when applied.
 *
 * Otherwise, let `T` be a `LambdaExpression` in a general form
 * `F<A1, A2, ...An>`, where `F` is a class template and `A1`, `A2`, ...`An`
 * are arbitrary types. The returned type is a non-template
 * `MetafunctionClass` without base class and equivalent to
   @code
        bind<
            quote<F>,
            bind_nested<A1>::type,
            bind_nested<A2>::type,
            bind_nested<An>::type...
        >
   @endcode
 * when applied. `bind_nested` is a metafunction returning a type such that
 * any nested `PlaceholderExpression` will be applied when the outer `bind`
 * is applied, but all other types are left unchanged.
 *
 *
 * @warning
 * Differences from the original MPL:
 * - The specification of the substitution mechanism is completely different,
 *   but most existing `LambdaExpression`s should work as-is.
 */
template <typename F>
struct lambda
    : if_<is_placeholder_expression<F>,
        lambda_detail::anonymous<F>,
        identity<F>
    >::type
{ };
}} // end namespace boost::mpl11

#endif // !BOOST_MPL11_LAMBDA_HPP
