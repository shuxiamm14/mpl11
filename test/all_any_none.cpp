/*!
 * @file
 * Contains unit tests for `boost::mpl11::all`, `boost::mpl11::any` and
 * `boost::mpl11::none`.
 */

#include <boost/mpl11/all.hpp>
#include <boost/mpl11/any.hpp>
#include <boost/mpl11/none.hpp>

#include <boost/mpl11/always.hpp>
#include <boost/mpl11/detail/is_same.hpp>
#include <boost/mpl11/detail/sequence_test.hpp>
#include <boost/mpl11/integral_c.hpp>
#include <boost/mpl11/partial.hpp>
#include <boost/mpl11/quote.hpp>


using namespace boost::mpl11;

struct x; struct y; struct z;

template <typename T>
using same_as = partial<quote<detail::is_same>, T>;

template <typename ...T>
using seq = detail::minimal_sequence<T...>;

// test all
static_assert( all<always<false_>, seq<>>::value, "");
static_assert(!all<always<false_>, seq<true_>>::value, "");
static_assert(!all<always<false_>, seq<true_, true_>>::value, "");

static_assert( all<always<true_>, seq<>>::value, "");
static_assert( all<always<true_>, seq<false_>>::value, "");
static_assert( all<always<true_>, seq<false_, false_>>::value, "");

static_assert( all<same_as<x>, seq<>>::value, "");
static_assert(!all<same_as<x>, seq<y>>::value, "");
static_assert(!all<same_as<x>, seq<y, z>>::value, "");
static_assert( all<same_as<x>, seq<x>>::value, "");
static_assert( all<same_as<x>, seq<x, x>>::value, "");
static_assert(!all<same_as<x>, seq<x, y>>::value, "");
static_assert(!all<same_as<x>, seq<y, x>>::value, "");
static_assert(!all<same_as<x>, seq<y, z, x>>::value, "");


// test any
static_assert(!any<always<false_>, seq<>>::value, "");
static_assert(!any<always<false_>, seq<true_>>::value, "");
static_assert(!any<always<false_>, seq<true_, true_>>::value, "");

static_assert(!any<always<true_>, seq<>>::value, "");
static_assert( any<always<true_>, seq<false_>>::value, "");
static_assert( any<always<true_>, seq<false_, false_>>::value, "");

static_assert(!any<same_as<x>, seq<>>::value, "");
static_assert(!any<same_as<x>, seq<y>>::value, "");
static_assert(!any<same_as<x>, seq<y, z>>::value, "");
static_assert( any<same_as<x>, seq<x>>::value, "");
static_assert( any<same_as<x>, seq<x, x>>::value, "");
static_assert( any<same_as<x>, seq<x, y>>::value, "");
static_assert( any<same_as<x>, seq<y, x>>::value, "");
static_assert( any<same_as<x>, seq<y, z, x>>::value, "");


// test none
static_assert(none<always<false_>, seq<>>::value, "");
static_assert(none<always<false_>, seq<true_>>::value, "");
static_assert(none<always<false_>, seq<true_, true_>>::value, "");

static_assert( none<always<true_>, seq<>>::value, "");
static_assert(!none<always<true_>, seq<false_>>::value, "");
static_assert(!none<always<true_>, seq<false_, false_>>::value, "");

static_assert( none<same_as<x>, seq<>>::value, "");
static_assert( none<same_as<x>, seq<y>>::value, "");
static_assert( none<same_as<x>, seq<y, z>>::value, "");
static_assert(!none<same_as<x>, seq<x>>::value, "");
static_assert(!none<same_as<x>, seq<x, x>>::value, "");
static_assert(!none<same_as<x>, seq<x, y>>::value, "");
static_assert(!none<same_as<x>, seq<y, x>>::value, "");
static_assert(!none<same_as<x>, seq<y, z, x>>::value, "");


int main() { }
