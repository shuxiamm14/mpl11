/*!
 * @file
 * Contains unit tests for `boost::mpl11::quote`.
 */

#include <boost/mpl11/quote.hpp>

#include <boost/mpl11/apply.hpp>
#include <boost/mpl11/detail/is_same.hpp>


using namespace boost::mpl11;
using detail::is_same;

struct a0; struct a1; struct a2;
template <typename ...> struct f { struct type; };

static_assert(is_same<
    apply_t<quote<f>>,
    f<>::type
>::value, "");

static_assert(is_same<
    apply_t<quote<f>, a0>,
    f<a0>::type
>::value, "");

static_assert(is_same<
    apply_t<quote<f>, a0, a1>,
    f<a0, a1>::type
>::value, "");

static_assert(is_same<
    apply_t<quote<f>, a0, a1, a2>,
    f<a0, a1, a2>::type
>::value, "");


int main() { }
