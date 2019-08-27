#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <assert.h>


#include "vlog.h"
#include "verror.h"
#include "vcat_containers.h"
#include "vstring.h"


#include "s11n.h"
#include "impl/metaargs_helper.h"

template <typename> class TD;

using namespace s11n;
using namespace std;


struct AAA {};
struct BBB {};
struct CCC {};

using T1 = std::tuple<char,int,long>;
using T2 = std::tuple<T1,double>;
using T3 = std::tuple<T1,T2,BBB,CCC>;
using T4 = std::tuple<T1,T2,T3>;
using T5 = std::tuple<T1,T2,T3,T4>;


namespace s11n {
    template<>
    struct Serial<AAA>
    {
        static constexpr auto name_of_type = "MyAAA";
        static constexpr auto description = "Ok";
        static std::tuple<int> to_tuple( const AAA& ) { return make_tuple(42); }
        //static int to_tuple( const AAA& ) { return {42}; }
    };
    template<>
    struct Serial<int>
    {
//        static constexpr auto name_of_type = "OwnInt";
//        static constexpr auto description = "DesInt";
//        static std::tuple<int> to_tuple( const int& ) { return {42}; }
    };
}

void foo(){}

template<class> class TD;
template<typename ... T> struct TTT {};
template<typename ... T> struct CCCC { explicit CCCC(int){} int foo() {return 3;} };

struct A
{
    char   c;
    bool   b;
    int32_t i;
};

template <class T>
struct Point
{
    T x, y, z;
};
using PointF = Point<float>;
using PointD = Point<double>;
using PointI = Point<int64_t>;

namespace s11n {
template<> struct Serial<A>
{
//    static constexpr auto name_of_type = "Own_A";
//    static constexpr auto description = "ver.1";

    static tuple<char,bool,int32_t> to_tuple(const A& a)
    {
        return make_tuple( a.c, a.b, a.i );
    }
};
}


template<typename T>
void prn()
{
    vdeb << calc_crc(signature<T>()) << signature_crc<T>() << signature<T>();
}

//template<typename R, typename ...Ts>
#include "impl/metaargs_helper.h"

int main()
{
    TTT<> t0;
    vdeb << impl::signature_metaargs( &t0 );

    vdeb << impl::is_metatype<A>();
    vdeb << signature<int>();
    vdeb << signature<A>();

    A a{0x31, true, 0x12345678};

    using TIII = tuple <int,int,int>;

    TIII t1{1,2,3};

    vdeb << VString(encode(a)).to_Hex();

    auto str = encode(t1);
    vdeb << VString( str ).to_Hex();
    auto dt1 = decode<decltype(t1)>( str );
    vdeb << get<0>(dt1) << get<1>(dt1) << get<2>(dt1);

    //TD<decltype(i)> ddd;
    //vdeb << signature<TTT<decltype(i)>>();
    //vdeb << signature<TTT<int*>>();
    //vdeb << signature<TTT<decltype(foo)>>();
}



