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



template<typename ... T> struct TTT {};
template<typename ... T> struct CCCC { explicit CCCC(int){} int foo() {} };
int main()
{
    vdeb << std::is_arithmetic<AAA>::value;
    vdeb << std::is_arithmetic<int>::value;
    //return 0;
    //vdeb << impl::has_serial_description<int>();
    //vdeb << impl::has_serial_description<AAA>();

    vdeb << signature<AAA>();
    //vdeb << crc<__int128>();
    //return 0;

    vdeb << signature< TTT<int,char,TTT<bool,bool,CCCC<uint,float>>>
                     >();
    vdeb << signature<TTT<>>();
}



