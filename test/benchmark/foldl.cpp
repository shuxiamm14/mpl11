/*!
 * @file
 * Comparison between using aliases and normal templates for a `foldl`
 * implementation.
 */

#define USE_ALIASES
// #define USE_NORMAL_RECURSION

#ifdef USE_ALIASES
    template <bool Done>
    struct until {
        template <typename F, typename State, typename Head, typename ...Tail>
        using apply = typename until<(sizeof...(Tail) == 0)>::
                      template apply<F, typename F::template apply<State, Head>::type, Tail...>;
    };

    template <>
    struct until<true> {
        template <typename F, typename State>
        struct apply {
            using type = State;
        };
    };

    template <typename F, typename State, typename ...T>
    struct foldl
        : until<sizeof...(T) == 0>::template apply<F, State, T...>
    { };

#elif defined(USE_NORMAL_RECURSION)

    template <typename F, typename State, typename ...T>
    struct foldl;

    template <typename F, typename State>
    struct foldl<F, State> {
        using type = State;
    };

    template <typename F, typename State, typename Head, typename ...Tail>
    struct foldl<F, State, Head, Tail...>
        : foldl<F, typename F::template apply<State, Head>::type, Tail...>
    { };

#endif

template <typename T, typename U>
struct is_same { static constexpr bool value = false; };

template <typename T>
struct is_same<T, T> { static constexpr bool value = true; };

struct f {
    template <typename State, typename T>
    struct apply {
        struct type;
    };
};

struct State;
template <int> struct x;

static_assert(is_same<
    foldl<f, State>::type,
    State
>::value, "");

static_assert(is_same<
    foldl<f, State, x<0>>::type,
    f::apply<State, x<0>>::type
>::value, "");

static_assert(is_same<
    foldl<f, State, x<0>, x<1>>::type,
    f::apply<f::apply<State, x<0>>::type, x<1>>::type
>::value, "");

static_assert(is_same<
    foldl<f, State, x<0>, x<1>, x<2>>::type,
    f::apply<f::apply<f::apply<State, x<0>>::type, x<1>>::type, x<2>>::type
>::value, "");

static_assert(is_same<
    foldl<f, State, x<0>, x<1>, x<2>, x<3>>::type,
    f::apply<f::apply<f::apply<f::apply<State, x<0>>::type, x<1>>::type, x<2>>::type, x<3>>::type
>::value, "");

/*

The reported times are the "user" part when running
time clang-3.4 -std=c++11 -Wall -Wextra -pedantic -o /dev/null foldl.cpp -ftemplate-depth=510
time clang++   -std=c++11 -Wall -Wextra -pedantic -o /dev/null foldl.cpp -ftemplate-depth=510
time g++-4.9   -std=c++11 -Wall -Wextra -pedantic -o /dev/null foldl.cpp

Commands were ran 5 times and the average was taken.


                     | clang v3.4 | Apple LLVM clang 5.0 | GCC 4.9.0 20131124 (experimental) |
----------------------------------------------------------------------------------------------
USE_ALIASES          |    0.20s   |         0.10s        |               0.16s               |
USE_NORMAL_RECURSION |    0.40s   |         0.18s        |               0.26s               |

*/

// ruby -e "puts (1..500).collect{|i| \"x<#{i}>\"}.join(', ')"
using Go = foldl<f, State,
    x<1>, x<2>, x<3>, x<4>, x<5>, x<6>, x<7>, x<8>, x<9>, x<10>, x<11>, x<12>, x<13>, x<14>, x<15>, x<16>, x<17>, x<18>, x<19>, x<20>, x<21>, x<22>, x<23>, x<24>, x<25>, x<26>, x<27>, x<28>, x<29>, x<30>, x<31>, x<32>, x<33>, x<34>, x<35>, x<36>, x<37>, x<38>, x<39>, x<40>, x<41>, x<42>, x<43>, x<44>, x<45>, x<46>, x<47>, x<48>, x<49>, x<50>, x<51>, x<52>, x<53>, x<54>, x<55>, x<56>, x<57>, x<58>, x<59>, x<60>, x<61>, x<62>, x<63>, x<64>, x<65>, x<66>, x<67>, x<68>, x<69>, x<70>, x<71>, x<72>, x<73>, x<74>, x<75>, x<76>, x<77>, x<78>, x<79>, x<80>, x<81>, x<82>, x<83>, x<84>, x<85>, x<86>, x<87>, x<88>, x<89>, x<90>, x<91>, x<92>, x<93>, x<94>, x<95>, x<96>, x<97>, x<98>, x<99>, x<100>, x<101>, x<102>, x<103>, x<104>, x<105>, x<106>, x<107>, x<108>, x<109>, x<110>, x<111>, x<112>, x<113>, x<114>, x<115>, x<116>, x<117>, x<118>, x<119>, x<120>, x<121>, x<122>, x<123>, x<124>, x<125>, x<126>, x<127>, x<128>, x<129>, x<130>, x<131>, x<132>, x<133>, x<134>, x<135>, x<136>, x<137>, x<138>, x<139>, x<140>, x<141>, x<142>, x<143>, x<144>, x<145>, x<146>, x<147>, x<148>, x<149>, x<150>, x<151>, x<152>, x<153>, x<154>, x<155>, x<156>, x<157>, x<158>, x<159>, x<160>, x<161>, x<162>, x<163>, x<164>, x<165>, x<166>, x<167>, x<168>, x<169>, x<170>, x<171>, x<172>, x<173>, x<174>, x<175>, x<176>, x<177>, x<178>, x<179>, x<180>, x<181>, x<182>, x<183>, x<184>, x<185>, x<186>, x<187>, x<188>, x<189>, x<190>, x<191>, x<192>, x<193>, x<194>, x<195>, x<196>, x<197>, x<198>, x<199>, x<200>, x<201>, x<202>, x<203>, x<204>, x<205>, x<206>, x<207>, x<208>, x<209>, x<210>, x<211>, x<212>, x<213>, x<214>, x<215>, x<216>, x<217>, x<218>, x<219>, x<220>, x<221>, x<222>, x<223>, x<224>, x<225>, x<226>, x<227>, x<228>, x<229>, x<230>, x<231>, x<232>, x<233>, x<234>, x<235>, x<236>, x<237>, x<238>, x<239>, x<240>, x<241>, x<242>, x<243>, x<244>, x<245>, x<246>, x<247>, x<248>, x<249>, x<250>, x<251>, x<252>, x<253>, x<254>, x<255>, x<256>, x<257>, x<258>, x<259>, x<260>, x<261>, x<262>, x<263>, x<264>, x<265>, x<266>, x<267>, x<268>, x<269>, x<270>, x<271>, x<272>, x<273>, x<274>, x<275>, x<276>, x<277>, x<278>, x<279>, x<280>, x<281>, x<282>, x<283>, x<284>, x<285>, x<286>, x<287>, x<288>, x<289>, x<290>, x<291>, x<292>, x<293>, x<294>, x<295>, x<296>, x<297>, x<298>, x<299>, x<300>, x<301>, x<302>, x<303>, x<304>, x<305>, x<306>, x<307>, x<308>, x<309>, x<310>, x<311>, x<312>, x<313>, x<314>, x<315>, x<316>, x<317>, x<318>, x<319>, x<320>, x<321>, x<322>, x<323>, x<324>, x<325>, x<326>, x<327>, x<328>, x<329>, x<330>, x<331>, x<332>, x<333>, x<334>, x<335>, x<336>, x<337>, x<338>, x<339>, x<340>, x<341>, x<342>, x<343>, x<344>, x<345>, x<346>, x<347>, x<348>, x<349>, x<350>, x<351>, x<352>, x<353>, x<354>, x<355>, x<356>, x<357>, x<358>, x<359>, x<360>, x<361>, x<362>, x<363>, x<364>, x<365>, x<366>, x<367>, x<368>, x<369>, x<370>, x<371>, x<372>, x<373>, x<374>, x<375>, x<376>, x<377>, x<378>, x<379>, x<380>, x<381>, x<382>, x<383>, x<384>, x<385>, x<386>, x<387>, x<388>, x<389>, x<390>, x<391>, x<392>, x<393>, x<394>, x<395>, x<396>, x<397>, x<398>, x<399>, x<400>, x<401>, x<402>, x<403>, x<404>, x<405>, x<406>, x<407>, x<408>, x<409>, x<410>, x<411>, x<412>, x<413>, x<414>, x<415>, x<416>, x<417>, x<418>, x<419>, x<420>, x<421>, x<422>, x<423>, x<424>, x<425>, x<426>, x<427>, x<428>, x<429>, x<430>, x<431>, x<432>, x<433>, x<434>, x<435>, x<436>, x<437>, x<438>, x<439>, x<440>, x<441>, x<442>, x<443>, x<444>, x<445>, x<446>, x<447>, x<448>, x<449>, x<450>, x<451>, x<452>, x<453>, x<454>, x<455>, x<456>, x<457>, x<458>, x<459>, x<460>, x<461>, x<462>, x<463>, x<464>, x<465>, x<466>, x<467>, x<468>, x<469>, x<470>, x<471>, x<472>, x<473>, x<474>, x<475>, x<476>, x<477>, x<478>, x<479>, x<480>, x<481>, x<482>, x<483>, x<484>, x<485>, x<486>, x<487>, x<488>, x<489>, x<490>, x<491>, x<492>, x<493>, x<494>, x<495>, x<496>, x<497>, x<498>, x<499>, x<500>
>::type;


int main() { }