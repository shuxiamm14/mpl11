/*!
 * @file
 * Contains unit tests for `boost::mpl11::into`.
 */

#include <boost/mpl11/into.hpp>

#include <boost/mpl11/apply.hpp>
#include <boost/mpl11/detail/is_same.hpp>


using namespace boost::mpl11;
using detail::is_same;

template <int> struct t;

template <typename ...> struct variadic;
static_assert(is_same<
    apply_t<into<variadic>>, variadic<>
>::value, "");
static_assert(is_same<
    apply_t<into<variadic>, t<0>>, variadic<t<0>>
>::value, "");
static_assert(is_same<
    apply_t<into<variadic>, t<0>, t<1>>, variadic<t<0>, t<1>>
>::value, "");
static_assert(is_same<
    apply_t<into<variadic>, t<0>, t<1>, t<2>>, variadic<t<0>, t<1>, t<2>>
>::value, "");

template <typename> struct unary;
static_assert(is_same<
    apply_t<into<unary>, t<0>>, unary<t<0>>
>::value, "");

template <typename = struct default_> struct unary_with_def;
static_assert(is_same<
    apply_t<into<unary_with_def>>, unary_with_def<struct default_>
>::value, "");
static_assert(is_same<
    apply_t<into<unary_with_def>, t<0>>, unary_with_def<t<0>>
>::value, "");

template <typename, typename> struct binary;
static_assert(is_same<
    apply_t<into<binary>, t<0>, t<1>>, binary<t<0>, t<1>>
>::value, "");


int main() { }
