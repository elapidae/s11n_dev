#include <iostream>

#include "vlog.h"
#include "verror.h"
#include "vcat_containers.h"
#include "s11n_serial.h"
#include "s11n_size.h"
#include "vstring.h"
#include "impl/tuple_helper.h"
#include "impl/signature.h"

#include "impl/crc.h"

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <assert.h>

#include "s11n_encoder.h"
#include "s11n_decoder.h"

#include "s11n.h"

//static void print_plain_name_of_type();

template <typename> class TD;

using namespace s11n::impl;
using namespace std;


struct AAA {};
struct BBB {};
struct CCC {};

using T1 = std::tuple<char,int,long>;
using T2 = std::tuple<T1,double>;
using T3 = std::tuple<T1,T2,BBB,CCC>;
using T4 = std::tuple<T1,T2,T3>;
using T5 = std::tuple<T1,T2,T3,T4>;


int main()
{
    //{char,int32,int64}
    vdeb.hex() << "signature:" << signature<tuple<>>() << ",crc:" << calc_crc(signature<tuple<>>());
    vdeb.hex() << "crc<tuple<>>():" << crc<tuple<>>();
    vdeb.hex() << "calc_crc({)" << calc_crc( "{" );
    vdeb.hex() << "calc_crc({})" << calc_crc( "{}" );
    //vdeb.hex() << signature<tuple<>>() << calc_crc(signature<tuple<>>()) ;
    vdeb;
    using ttt = tuple<char,bool>;
    vdeb.hex() << "signature:" << signature<ttt>() << ",crc:" << calc_crc(signature<ttt>());
    vdeb.hex() << "crc<tuple<>>():" << crc<ttt>();
    vdeb.hex() << "calc_crc({)" << calc_crc( "{" );
    vdeb.hex() << "calc_crc({})" << calc_crc( "{}" );
    return 0;

    auto c = crc<T1>();
    vdeb << signature<T1>();
    vdeb.hex() << calc_crc(signature<T1>()) << crc<T1>();
    vdeb.hex() << calc_crc(signature<T5>()) << crc<T5>();
    return 0;

    using namespace s11n::impl;

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



