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
}

template<typename T>
struct TTT
{};

template<typename T, typename Sub, typename ... >
struct _is_any_metatype : public false_type {};

//template<typename T>
//struct _is_any_metatype<T, T<Sub>> : public false_type {};

template< typename...>
constexpr bool is_metatype()
{
    return false;
}

template< template<typename...> class MetaT, typename ... Args >
constexpr bool is_metatype()
{
    return true;
}



int main()
{
    vdeb << signature<TTT<int>>();
    vdeb << is_metatype<int>();
    vdeb << is_metatype<TTT>();

//    vdeb.hex() << calc_crc(signature<AAA>()) << crc<AAA>();
//    vdeb << impl::is_tuple<tuple<int>>();
//    vdeb << impl::_has_serial_tuple<AAA>();
//    vdeb << impl::_has_serial_tuple<BBB>();
    return 0;

    using T = set<vector<map<int,set<tuple<int,uint>>>>>;
    vdeb << impl::signature<T>();
    return 0;

    //auto c = crc<T1>();
    vdeb << signature<T1>();
    vdeb.hex() << calc_crc(signature<T1>()) << crc<T1>();
    vdeb.hex() << calc_crc(signature<T5>()) << crc<T5>();
    return 0;

    //vdeb << name_of_type_from_PF<int>();
    //vdeb << name_of_type_from_PF<vector<int>>();

    vdeb << signature<int>();
    vdeb << signature<vector<int>>();

    vdeb << signature<AAA>();
    vdeb << signature<BBB>();
    vdeb << signature<CCC>();
    vdeb << signature<T1>();
    vdeb << signature<T2>();
    vdeb << signature<T3>();
    vdeb << signature<T4>();
    vdeb << signature<T5>();

    vdeb.hex() << calc_crc(signature<AAA>()) << crc<AAA>();
    vdeb.hex() << calc_crc(signature<T1>()) << crc<T1>();
}



