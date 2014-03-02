/*!
 * @file
 * Contains unit tests for @ref List.
 */

#include <boost/mpl11/list.hpp>

#include <boost/mpl11/bool.hpp>
#include <boost/mpl11/core.hpp>
#include <boost/mpl11/functional.hpp>
#include <boost/mpl11/integer.hpp>

#include "check_finite_iterable.hpp"
#include "minimal_iterable.hpp"


using namespace boost::mpl11;

namespace test_list {
    template <int ...xs>
    struct list_of
        : check_finite_iterable<
            list<int_<xs>...>,
            int_<xs>...
        >

        , check_finite_iterable<
            cons<int_<-1>, minimal_iterable<int_<xs>...>>,
            int_<-1>, int_<xs>...
        >
        , check_finite_iterable<
            cons<int_<-1>, list<int_<xs>...>>,
            int_<-1>, int_<xs>...
        >
    { };

    struct tests
        : list_of<>
        , list_of<0>
        , list_of<0, 1>
        , list_of<0, 1, 2>
        , list_of<0, 1, 2, 3>
        , list_of<0, 1, 2, 3, 4>
        , list_of<0, 1, 2, 3, 4, 5>
    { };
} // end namespace test_list

namespace test_snoc {
    // note: we could test snoc automatically, but clang 3.5 fails with an
    // internal error when we do something like `int_<xs>..., int_<last>`.
    template <int x>
    struct appending {
        template <int ...xs>
        struct to {
            template <int ...ys>
            struct is
                : appending
                , check_finite_iterable<
                    snoc<minimal_iterable<int_<xs>...>, int_<x>>,
                    int_<ys>...
                >
                , check_finite_iterable<
                    snoc<list<int_<xs>...>, int_<x>>,
                    int_<ys>...
                >
            { };
        };
    };

    struct tests
        : appending<-1>
            ::to<>::is<-1>
            ::to<0>::is<0, -1>
            ::to<0, 1>::is<0, 1, -1>
            ::to<0, 1, 2>::is<0, 1, 2, -1>
            ::to<0, 1, 2, 3>::is<0, 1, 2, 3, -1>
    { };
} // end namespace test_snoc

namespace test_functor {
    template <typename> struct f { struct type; };

    template <int ...xs>
    struct do_fmap
        : check_finite_iterable<
            fmap<quote<f>, list<int_<xs>...>>,
            f<int_<xs>>...
        >
    { };

    struct tests
        : do_fmap<>
        , do_fmap<0>
        , do_fmap<0, 1>
        , do_fmap<0, 1, 2>
        , do_fmap<0, 1, 2, 3>
        , do_fmap<0, 1, 2, 3, 4>
        , do_fmap<0, 1, 2, 3, 4, 5>
    { };
} // end namespace test_functor

namespace test_take {
    template <unsigned long n>
    struct taking {
        template <int ...all>
        struct from {
            template <int ...kept>
            struct is
                : check_finite_iterable<
                    take_c<n, minimal_iterable<int_<all>...>>,
                    int_<kept>...
                >
            { };
        };
    };

    struct tests
        : taking<0>::from<>::is<>
        , taking<1>::from<>::is<>
        , taking<2>::from<>::is<>
        , taking<3>::from<>::is<>

        , taking<0>::from<0>::is<>
        , taking<1>::from<0>::is<0>
        , taking<2>::from<0>::is<0>
        , taking<3>::from<0>::is<0>

        , taking<0>::from<0, 1>::is<>
        , taking<1>::from<0, 1>::is<0>
        , taking<2>::from<0, 1>::is<0, 1>
        , taking<3>::from<0, 1>::is<0, 1>

        , taking<0>::from<0, 1, 2>::is<>
        , taking<1>::from<0, 1, 2>::is<0>
        , taking<2>::from<0, 1, 2>::is<0, 1>
        , taking<3>::from<0, 1, 2>::is<0, 1, 2>
        , taking<4>::from<0, 1, 2>::is<0, 1, 2>
        , taking<5>::from<0, 1, 2>::is<0, 1, 2>

        , taking<0>::from<0, 1, 2, 3>::is<>
        , taking<1>::from<0, 1, 2, 3>::is<0>
        , taking<2>::from<0, 1, 2, 3>::is<0, 1>
        , taking<3>::from<0, 1, 2, 3>::is<0, 1, 2>
        , taking<4>::from<0, 1, 2, 3>::is<0, 1, 2, 3>
        , taking<5>::from<0, 1, 2, 3>::is<0, 1, 2, 3>
    { };
} // end namespace test_take

namespace test_repeat {
    template <unsigned long n, int x>
    struct repeating {
        template <int ...xs>
        struct is
            : check_finite_iterable<
                take_c<n, repeat<int_<x>>>,
                int_<xs>...
            >
        { };
    };

    struct tests
        : repeating<0, 0>::is<>
        , repeating<1, 0>::is<0>
        , repeating<2, 0>::is<0, 0>
        , repeating<3, 0>::is<0, 0, 0>
        , repeating<4, 0>::is<0, 0, 0, 0>
        , repeating<5, 0>::is<0, 0, 0, 0, 0>
    { };
} // end namespace test_repeat

namespace test_iterate {
    template <typename x>
    struct f { using type = f<typename x::type>; };
    struct x { using type = x; };

    template <unsigned long n>
    struct iterating {
        template <typename ...fxs>
        struct is
            : check_finite_iterable<
                take_c<n, iterate<quote<f>, x>>,
                fxs...
            >
        { };
    };

    struct tests
        : iterating<1>::is<x>
        , iterating<2>::is<x, f<x>>
        , iterating<3>::is<x, f<x>, f<f<x>>>
        , iterating<4>::is<x, f<x>, f<f<x>>, f<f<f<x>>>>
        , iterating<5>::is<x, f<x>, f<f<x>>, f<f<f<x>>>, f<f<f<f<x>>>>>
    { };
} // end namespace test_iterate

namespace test_take_while {
    struct nonzero {
        using type = nonzero;
        template <typename n>
        using apply = bool_<n::type::value != 0>;
    };

    template <int ...all>
    struct nonzero_prefix {
        template <int ...prefix>
        struct is
            : check_finite_iterable<
                take_while<nonzero, minimal_iterable<int_<all>...>>,
                int_<prefix>...
            >
        { };
    };

    struct tests
        : nonzero_prefix<>::is<>

        , nonzero_prefix<0>::is<>
        , nonzero_prefix<1>::is<1>

        , nonzero_prefix<0, 0>::is<>
        , nonzero_prefix<0, 1>::is<>
        , nonzero_prefix<1, 0>::is<1>
        , nonzero_prefix<1, 2>::is<1, 2>

        , nonzero_prefix<0, 0, 0>::is<>
        , nonzero_prefix<0, 0, 1>::is<>
        , nonzero_prefix<0, 1, 0>::is<>
        , nonzero_prefix<0, 1, 2>::is<>
        , nonzero_prefix<1, 0, 0>::is<1>
        , nonzero_prefix<1, 0, 2>::is<1>
        , nonzero_prefix<1, 2, 0>::is<1, 2>
        , nonzero_prefix<1, 2, 3>::is<1, 2, 3>
    { };
} // end namespace take_while

namespace test_reverse {
    template <int ...xs>
    struct reversed {
        template <int ...rev>
        struct is
            : check_finite_iterable<
                reverse<minimal_iterable<int_<xs>...>>,
                int_<rev>...
            >
        { };
    };

    struct tests
        : reversed<>::is<>
        , reversed<0>::is<0>
        , reversed<0, 1>::is<1, 0>
        , reversed<0, 1, 2>::is<2, 1, 0>
        , reversed<0, 1, 2, 3>::is<3, 2, 1, 0>
        , reversed<0, 1, 2, 3, 4>::is<4, 3, 2, 1, 0>
    { };
} // end namespace test_reverse

namespace test_filter {
    template <int ...all>
    struct nonzeros {
        template <int ...filtered>
        struct are
            : check_finite_iterable<
                filter<quote<id>, minimal_iterable<int_<all>...>>,
                int_<filtered>...
            >
        { };
    };

    struct tests
        : nonzeros<>::are<>

        , nonzeros<0>::are<>
        , nonzeros<1>::are<1>

        , nonzeros<0, 0>::are<>
        , nonzeros<1, 0>::are<1>
        , nonzeros<0, 2>::are<2>
        , nonzeros<1, 2>::are<1, 2>

        , nonzeros<0, 0, 0>::are<>
        , nonzeros<0, 0, 3>::are<3>
        , nonzeros<0, 2, 0>::are<2>
        , nonzeros<0, 2, 3>::are<2, 3>
        , nonzeros<1, 0, 0>::are<1>
        , nonzeros<1, 0, 3>::are<1, 3>
        , nonzeros<1, 2, 0>::are<1, 2>
        , nonzeros<1, 2, 3>::are<1, 2, 3>
    { };
} // end namespace test_filter

namespace test_scanl {
    template <typename x, typename y>
    struct f { using type = f<typename x::type, typename y::type>; };

    template <int state, int ...xs>
    struct scanning {
        template <typename ...result>
        struct is
            : check_finite_iterable<
                scanl<quote<f>, int_<state>, minimal_iterable<int_<xs>...>>,
                result...
            >
        { };
    };

    struct tests
        : scanning<0>::is<
            int_<0>
        >

        , scanning<0, 1>::is<
            int_<0>,
            f<int_<0>, int_<1>>
        >

        , scanning<0, 1, 2>::is<
            int_<0>,
            f<int_<0>, int_<1>>,
            f<f<int_<0>, int_<1>>, int_<2>>
        >

        , scanning<0, 1, 2, 3>::is<
            int_<0>,
            f<int_<0>, int_<1>>,
            f<f<int_<0>, int_<1>>, int_<2>>,
            f<f<f<int_<0>, int_<1>>, int_<2>>, int_<3>>
        >

        , scanning<0, 1, 2, 3, 4>::is<
            int_<0>,
            f<int_<0>, int_<1>>,
            f<f<int_<0>, int_<1>>, int_<2>>,
            f<f<f<int_<0>, int_<1>>, int_<2>>, int_<3>>,
            f<f<f<f<int_<0>, int_<1>>, int_<2>>, int_<3>>, int_<4>>
        >

        , scanning<0, 1, 2, 3, 4, 5>::is<
            int_<0>,
            f<int_<0>, int_<1>>,
            f<f<int_<0>, int_<1>>, int_<2>>,
            f<f<f<int_<0>, int_<1>>, int_<2>>, int_<3>>,
            f<f<f<f<int_<0>, int_<1>>, int_<2>>, int_<3>>, int_<4>>,
            f<f<f<f<f<int_<0>, int_<1>>, int_<2>>, int_<3>>, int_<4>>, int_<5>>
        >
    { };
} // end namespace test_scanl

namespace test_zip {
    template <typename ...lists>
    struct zipping {
        template <typename ...zipped>
        struct is
            : check_finite_iterable<zip<lists...>, zipped...>
        { };
    };

    template <typename ...xs>
    using seq = list<box<xs>...>;

    struct w0; struct w1; struct w2; struct w3; struct w4;
    struct x0; struct x1; struct x2; struct x3; struct x4;
    struct y0; struct y1; struct y2; struct y3; struct y4;
    struct z0; struct z1; struct z2; struct z3; struct z4;

    struct tests
        // zip 0 iterables
        : zipping<>::is<>

        // zip 1 iterable
        , zipping<seq<x0>>::is<seq<x0>>
        , zipping<seq<x0, x1>>::is<seq<x0>, seq<x1>>
        , zipping<seq<x0, x1, x2>>::is<seq<x0>, seq<x1>, seq<x2>>

        // zip 2 iterables
        , zipping<seq<>, seq<>>::is<>
        , zipping<seq<>, seq<x0>>::is<>
        , zipping<seq<>, seq<x0, x1>>::is<>

        , zipping<seq<w0>, seq<>>::is<>
        , zipping<seq<w0>, seq<x0>>::is<seq<w0, x0>>
        , zipping<seq<w0>, seq<x0, x1>>::is<seq<w0, x0>>

        , zipping<seq<w0, w1>, seq<>>::is<>
        , zipping<seq<w0, w1>, seq<x0>>::is<seq<w0, x0>>
        , zipping<seq<w0, w1>, seq<x0, x1>>::is<seq<w0, x0>, seq<w1, x1>>

        // zip 3 iterables
        , zipping<seq<>, seq<>, seq<y0>>::is<>
        , zipping<seq<>, seq<x0>, seq<>>::is<>
        , zipping<seq<>, seq<x0>, seq<y0>>::is<>
        , zipping<seq<w0>, seq<>, seq<>>::is<>
        , zipping<seq<w0>, seq<>, seq<y0>>::is<>
        , zipping<seq<w0>, seq<x0>, seq<>>::is<>
        , zipping<seq<w0>, seq<x0>, seq<y0>>::is<seq<w0, x0, y0>>

        // zip 4 iterables with (sometimes) different sizes
        , zipping<
            seq<w0, w1, w2, w3>,
            seq<x0, x1, x2, x3, x4>,
            seq<y0, y1, y2>,
            seq<z0, z1, z2, z3, z4>
        >::is<
            seq<w0, x0, y0, z0>,
            seq<w1, x1, y1, z1>,
            seq<w2, x2, y2, z2>
        >
    { };
} // end namespace test_zip

namespace test_join {
    template <int ...xs>
    using seq = minimal_iterable<int_<xs>...>;

    template <typename ...iterables>
    struct joining {
        template <int ...joined>
        struct is
            : check_finite_iterable<join<iterables...>, int_<joined>...>
        { };
    };

    template <int ...xs>
    struct auto_test_join
        : joining<seq<xs...>>::template is<xs...>

        , joining<seq<xs...>, seq<>>::template is<xs...>
        , joining<seq<xs...>, seq<0>>::template is<xs..., 0>
        , joining<seq<xs...>, seq<0, 1>>::template is<xs..., 0, 1>

        , joining<seq<xs...>, seq<>, seq<>>::template is<xs...>
        , joining<seq<xs...>, seq<>, seq<0>>::template is<xs..., 0>
        , joining<seq<xs...>, seq<>, seq<0, 1>>::template is<xs..., 0, 1>

        , joining<seq<xs...>, seq<0>, seq<>>::template is<xs..., 0>
        , joining<seq<xs...>, seq<0>, seq<1>>::template is<xs..., 0, 1>
        , joining<seq<xs...>, seq<0>, seq<1, 2>>::template is<xs..., 0, 1, 2>

        , joining<seq<xs...>, seq<0, 1>, seq<>>::template is<xs..., 0, 1>
        , joining<seq<xs...>, seq<0, 1>, seq<2>>::template is<xs..., 0, 1, 2>
        , joining<seq<xs...>, seq<0, 1>, seq<2, 3>>::template is<xs..., 0, 1, 2, 3>
    { };

    struct tests
        : joining<>::is<>
        , auto_test_join<>
        , auto_test_join<9>
        , auto_test_join<9, 8>
        , auto_test_join<9, 8, 7>
        , auto_test_join<9, 8, 7, 6>
        , joining<
            seq<0>, seq<1>, seq<>, seq<2, 3>, seq<>, seq<4, 5>, seq<6>, seq<>
        >::is<0, 1, 2, 3, 4, 5, 6>
    { };
} // end namespace test_join

namespace test_slice {
    template <int ...xs>
    struct slicing {
        template <unsigned long start, unsigned long stop>
        struct at {
            template <int ...sliced>
            struct is
                : check_finite_iterable<
                    slice_c<minimal_iterable<int_<xs>...>, start, stop>,
                    int_<sliced>...
                >
            { };
        };
    };

    struct tests
        : slicing<>::at<0, 0>::is<>

        , slicing<0>::at<0, 0>::is<>
        , slicing<0>::at<0, 1>::is<0>
        , slicing<0>::at<1, 1>::is<>

        , slicing<0, 1>::at<0, 0>::is<>
        , slicing<0, 1>::at<0, 1>::is<0>
        , slicing<0, 1>::at<0, 2>::is<0, 1>
        , slicing<0, 1>::at<1, 1>::is<>
        , slicing<0, 1>::at<1, 2>::is<1>
        , slicing<0, 1>::at<2, 2>::is<>

        , slicing<0, 1, 2>::at<0, 0>::is<>
        , slicing<0, 1, 2>::at<0, 1>::is<0>
        , slicing<0, 1, 2>::at<0, 2>::is<0, 1>
        , slicing<0, 1, 2>::at<0, 3>::is<0, 1, 2>
        , slicing<0, 1, 2>::at<1, 1>::is<>
        , slicing<0, 1, 2>::at<1, 2>::is<1>
        , slicing<0, 1, 2>::at<1, 3>::is<1, 2>
        , slicing<0, 1, 2>::at<2, 2>::is<>
        , slicing<0, 1, 2>::at<2, 3>::is<2>
        , slicing<0, 1, 2>::at<3, 3>::is<>
    { };
} // end namespace test_slice

namespace test_sort {
    template <typename pred, int ...unsorted>
    struct sorting_by {
        template <int ...sorted>
        struct is
            : check_finite_iterable<
                sort_by<pred, minimal_iterable<int_<unsorted>...>>,
                int_<sorted>...
            >
        { };
    };

    template <typename Less>
    struct sort_less
        : sorting_by<Less>::template is<>
        , sorting_by<Less, 0>::template is<0>
        , sorting_by<Less, 0, 1>::template is<0, 1>
        , sorting_by<Less, 1, 0>::template is<0, 1>
        , sorting_by<Less, 0, 1, 2>::template is<0, 1, 2>
        , sorting_by<Less, 1, 0, 2>::template is<0, 1, 2>
        , sorting_by<Less, 5, 1, 0, 2, 3, 0>::template is<0, 0, 1, 2, 3, 5>
    { };

    struct tests
        : sort_less<quote<less>>
        , sort_less<quote<less_equal>>
    { };
} // end namespace test_sort

namespace test_init {
    template <int ...xs>
    struct init_of {
        template <int ...ys>
        struct is
            : check_finite_iterable<
                init<minimal_iterable<int_<xs>...>>,
                int_<ys>...
            >
        { };
    };

    struct tests
        : init_of<0>::is<>
        , init_of<0, 1>::is<0>
        , init_of<0, 1, 2>::is<0, 1>
        , init_of<0, 1, 2, 3>::is<0, 1, 2>
        , init_of<0, 1, 2, 3, 4>::is<0, 1, 2, 3>
        , init_of<0, 1, 2, 3, 4, 5>::is<0, 1, 2, 3, 4>
    { };
} // end namespace test_init


int main() { }