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


//static void print_plain_name_of_type();

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
        static std::tuple<int> to_tuple( const AAA& ) { return {42}; }
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

int main()
{
    using T = tuple<int16_t, char, bool>;
    using V = vector<uint64_t>;
    using T2 = tuple<T,V,A>;
    using C = map<int,T2>;
    using UM = unordered_map<int,C>;
    using T3 = tuple<C,T2,V,T,UM>;

    vdeb << signature<T3>();

    vdeb << signature_crc<T3>() << calc_crc( signature<T3>() );
    vdeb;
    vdeb;
    vdeb;
    prn<T>();
    prn<V>();
    prn<A>();
    vdeb;
    vdeb;
    prn<C>();
    prn<T2>();
    prn<V>();
    prn<T>();
    prn<UM>();
    return 0;

    //TD<decltype(i)> ddd;
    //vdeb << signature<TTT<decltype(i)>>();
    //vdeb << signature<TTT<int*>>();
    //vdeb << signature<TTT<decltype(foo)>>();
}



