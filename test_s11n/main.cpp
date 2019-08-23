/****************************************************************************************
**
**  VLIBS codebase, NIIAS
**
**  Authors:
**  Alexandre Gromtsev aka elapidae     elapidae@yandex.ru
**  Nadezhda Churikova aka claorisel    claorisel@gmail.com
**  Ekaterina Boltenkova aka kataretta  kitkat52@yandex.ru
**  Ivan Deylid aka sid1057             ivanov.dale@gmail.com>
**
**  GNU Lesser General Public License Usage
**  This file may be used under the terms of the GNU Lesser General Public License
**  version 3 as published by the Free Software Foundation and appearing in the file
**  LICENSE.LGPL3 included in the packaging of this file. Please review the following
**  information to ensure the GNU Lesser General Public License version 3 requirements
**  will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
****************************************************************************************/


#include "gtest/gtest.h"

#include "s11n.h"

#include <vector>
#include <deque>
#include <array>
#include <valarray>

#include <list>
#include <forward_list>

#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

// For pretty printing.
#include <iostream>
#define vdeb std::cout << __FILE__ << ":" << __LINE__ << "==> "


using namespace std;
using namespace s11n;


//=======================================================================================
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wweak-vtables"
class Test_s11n: public testing::Test {};
#pragma GCC diagnostic pop
//=======================================================================================


//=======================================================================================
TEST_F( Test_s11n, plain_names )
{
    EXPECT_EQ( signature<bool>(),     "bool"    );
    EXPECT_EQ( signature<char>(),     "char"    );

    EXPECT_EQ( signature<int8_t>(),   "int8"    );
    EXPECT_EQ( signature<int16_t>(),  "int16"   );
    EXPECT_EQ( signature<int32_t>(),  "int32"   );
    EXPECT_EQ( signature<int64_t>(),  "int64"   );

    EXPECT_EQ( signature<uint8_t>(),  "uint8"   );
    EXPECT_EQ( signature<uint16_t>(), "uint16"  );
    EXPECT_EQ( signature<uint32_t>(), "uint32"  );
    EXPECT_EQ( signature<uint64_t>(), "uint64"  );

    EXPECT_EQ( signature<float>(),    "float32" );
    EXPECT_EQ( signature<double>(),   "float64" );
}
//=======================================================================================

//=======================================================================================
template<typename T>
crc_type own_crc()
{
    return calc_crc( signature<T>() );
}
TEST_F( Test_s11n, plain_crcs )
{
    EXPECT_EQ( signature_crc<bool>(),       own_crc<bool>()     );
    EXPECT_EQ( signature_crc<char>(),       own_crc<char>()     );

    EXPECT_EQ( signature_crc<int8_t>(),     own_crc<int8_t>()   );
    EXPECT_EQ( signature_crc<int16_t>(),    own_crc<int16_t>()  );
    EXPECT_EQ( signature_crc<int32_t>(),    own_crc<int32_t>()  );
    EXPECT_EQ( signature_crc<int64_t>(),    own_crc<int64_t>()  );

    EXPECT_EQ( signature_crc<uint8_t>(),    own_crc<uint8_t>()  );
    EXPECT_EQ( signature_crc<uint16_t>(),   own_crc<uint16_t>() );
    EXPECT_EQ( signature_crc<uint32_t>(),   own_crc<uint32_t>() );
    EXPECT_EQ( signature_crc<uint64_t>(),   own_crc<uint64_t>() );

    EXPECT_EQ( signature_crc<float>(),      own_crc<float>()    );
    EXPECT_EQ( signature_crc<double>(),     own_crc<double>()   );
}
//=======================================================================================


//=======================================================================================
namespace test_ns
{
    struct OwnType {};
}
TEST_F( Test_s11n, own_name_of_type )
{
    EXPECT_EQ( signature<test_ns::OwnType>(), "test_ns::OwnType"  );

    EXPECT_EQ( signature_crc<test_ns::OwnType>(), own_crc<test_ns::OwnType>() );
}
//=======================================================================================


//=======================================================================================
template <class T>
struct Point
{
    T x, y, z;
};
using PointF = Point<float>;
using PointD = Point<double>;
using PointI = Point<int64_t>;

template <typename ...>
struct MetaArgs {};

TEST_F( Test_s11n, name_of_metatypes )
{
    EXPECT_EQ( signature<PointF>(), "Point<float32>"  );
    EXPECT_EQ( signature<PointD>(), "Point<float64>"  );
    EXPECT_EQ( signature<PointI>(), "Point<int64>"    );

    EXPECT_EQ( signature_crc<PointF>(), calc_crc("Point<float32>")  );
    EXPECT_EQ( signature_crc<PointD>(), calc_crc("Point<float64>")  );
    EXPECT_EQ( signature_crc<PointI>(), calc_crc("Point<int64>")    );

    using MA0 = MetaArgs<>;
    using MA1 = MetaArgs<int32_t>;
    using MA3 = MetaArgs<bool,char,test_ns::OwnType>;
    EXPECT_EQ( signature_crc<MA0>(), calc_crc("MetaArgs<>") );
    EXPECT_EQ( signature_crc<MA1>(), calc_crc("MetaArgs<int32>") );
    EXPECT_EQ( signature_crc<MA3>(), calc_crc("MetaArgs<bool,char,test_ns::OwnType>") );
}
//=======================================================================================


//=======================================================================================
TEST_F( Test_s11n, name_of_containers )
{
    EXPECT_EQ( signature< string >(), "std::string" );

    EXPECT_EQ( signature< vector<bool>          >(), "std::vector<bool>"        );
    EXPECT_EQ( signature< deque<bool>           >(), "std::deque<bool>"         );
    EXPECT_EQ( signature< valarray<bool>        >(), "std::valarray<bool>"      );
    EXPECT_EQ( signature< set<bool>             >(), "std::set<bool>"           );
    EXPECT_EQ( signature< unordered_set<bool>   >(), "std::unordered_set<bool>" );


    // Надо делать using, иначе ломается макрос сравнения.
    using AT  = array<bool,13>;
    using MT  = map<int8_t,bool>;
    using UMT = unordered_map<int8_t,bool>;

    EXPECT_EQ( signature< AT  >(), "std::array<bool>"                );
    EXPECT_EQ( signature< MT  >(), "std::map<{int8,bool}>"           );
    EXPECT_EQ( signature< UMT >(), "std::unordered_map<{int8,bool}>" );

    // TODO: Надо думать как сделать частичную перегрузку для шаблонных типов.
    // Надо чтобы можно было перегрузить Serial для имени типа, но оставить в покое
    // вывод шаблонных аргументов. Т.е.:
    //
    //  template<typename T> Serial<std::list<T>>
    //  {
    //      ??? name_of_type = ???
    //  };
    //
    //vdeb << signature< list<bool> >(); // std::__cxx11::list<bool>
    //EXPECT_EQ( signature< list<bool> >(), "std::list<bool>" );

    // "Не является" контейнером, т.к. не имеет метода size(). Пишите письма...
    // vdeb << signature< forward_list<bool> >();
    // EXPECT_EQ( signature< forward_list<bool> >(), "std::forward_list<bool>"    );
}
//=======================================================================================
TEST_F( Test_s11n, crcs_of_containers )
{
    EXPECT_EQ( signature_crc< string >(), own_crc<string>() );

    // Надо делать using, иначе ломается макрос сравнения.
    using V  = vector<test_ns::OwnType>;
    using D  = deque<test_ns::OwnType>;
    using VA = valarray<test_ns::OwnType>;
    using S  = set<int64_t>;
    using US = unordered_set<uint32_t>;
    using A  = array<test_ns::OwnType,13>;
    using M  = map<int8_t,test_ns::OwnType>;
    using UM = unordered_map<int8_t,test_ns::OwnType>;

    EXPECT_EQ( signature_crc< V  >(), own_crc< V  >() );
    EXPECT_EQ( signature_crc< D  >(), own_crc< D  >() );
    EXPECT_EQ( signature_crc< VA >(), own_crc< VA >() );
    EXPECT_EQ( signature_crc< S  >(), own_crc< S  >() );
    EXPECT_EQ( signature_crc< US >(), own_crc< US >() );
    EXPECT_EQ( signature_crc< A  >(), own_crc< A  >() );
    EXPECT_EQ( signature_crc< M  >(), own_crc< M  >() );
    EXPECT_EQ( signature_crc< UM >(), own_crc< UM >() );
}
//=======================================================================================


//=======================================================================================
using Tup0 = tuple<>;
using Tup1b = tuple<bool>;
using Tup1u64 = tuple<uint64_t>;
using TupT = tuple<Tup0,Tup1b,Tup1u64,char,bool,uint16_t,
                   test_ns::OwnType,
                   MetaArgs<Tup1b>>;

TEST_F( Test_s11n, tuple_names )
{
    EXPECT_EQ( signature<Tup0>(), "{}" );
    EXPECT_EQ( signature_crc<Tup0>(), calc_crc("{}") );

    EXPECT_EQ( signature<Tup1b>(), "{bool}" );
    EXPECT_EQ( signature_crc<Tup1b>(), calc_crc("{bool}") );

    EXPECT_EQ( signature<Tup1u64>(), "{uint64}" );
    EXPECT_EQ( signature_crc<Tup1u64>(), calc_crc("{uint64}") );

    string sign_TT = "{{},{bool},{uint64},char,bool,uint16,"
                      "test_ns::OwnType,"
                      "MetaArgs<{bool}>"
                     "}";
    EXPECT_EQ( signature<TupT>(), sign_TT );
    EXPECT_EQ( signature_crc<TupT>(), calc_crc(sign_TT) );
}
//=======================================================================================


//=======================================================================================
struct Redefined_name_of_type {};

namespace s11n
{
    template<> struct Serial<Redefined_name_of_type>
    {
        static constexpr auto name_of_type = "My redefined name of type";
    };
} // s11n namespace

TEST_F( Test_s11n, Redefined_name_of_type )
{
    EXPECT_EQ( signature<Redefined_name_of_type>(),
               "My redefined name of type" );

    EXPECT_EQ( signature_crc<Redefined_name_of_type>(),
               calc_crc("My redefined name of type") );
}
//=======================================================================================


//=======================================================================================
struct Defined_description {};

namespace s11n{
    template<> struct Serial<Defined_description>
    {
        static constexpr auto description = "My description, ver. 1";
    };
} // s11n namespace

TEST_F( Test_s11n, Defined_description )
{
    auto sign = "Defined_description(My description, ver. 1)";
    EXPECT_EQ( signature<Defined_description>(), sign );

    EXPECT_EQ( signature_crc<Defined_description>(), calc_crc(sign) );
}
//=======================================================================================

//=======================================================================================
struct Both_Name_And_Description {};

namespace s11n {
    template<> struct Serial<Both_Name_And_Description>
    {
        static constexpr auto name_of_type = "Имя";
        static constexpr auto description = "Описание";
    };
} // s11n namespace

TEST_F( Test_s11n, Both_name_description )
{
    auto sign = "Имя(Описание)";
    EXPECT_EQ( signature<Both_Name_And_Description>(), sign );
    EXPECT_EQ( signature_crc<Both_Name_And_Description>(), calc_crc(sign) );
}
//=======================================================================================


//=======================================================================================
//  For lulzzz ^_^
TEST_F( Test_s11n, some_random_signature )
{

    using T = tuple<int16_t, char, bool>;
    using V = vector<T>;
    using T2 = tuple<T,V,test_ns::OwnType>;
    using C = map<int,T2>;
    using UM = unordered_map<int,C>;
    using T3 = tuple<C,T2,V,T,UM>;
    using S = set<char>;
    using T4 = tuple<T,V,T2,C,UM,T3,S>;

//    vdeb <<hex  << endl << endl << endl
//          << signature<T4>()
//          << endl << endl << endl << endl;

    auto sign = "{{int16,char,bool},std::vector<{int16,char,bool}>,"
                "{{int16,char,bool},std::vector<{int16,char,bool}>,"
                "test_ns::OwnType},std::map<{int32,{{int16,char,bool},"
                "std::vector<{int16,char,bool}>,test_ns::OwnType}}>,"
                "std::unordered_map<{int32,std::map<{int32,{{int16,char,bool},"
                "std::vector<{int16,char,bool}>,test_ns::OwnType}}>}>,"
                "{std::map<{int32,{{int16,char,bool},std::vector<{int16,char,bool}>,"
                "test_ns::OwnType}}>,{{int16,char,bool},std::vector<{int16,char,bool}>,"
                "test_ns::OwnType},std::vector<{int16,char,bool}>,"
                "{int16,char,bool},std::unordered_map<{int32,"
                "std::map<{int32,{{int16,char,bool},"
                "std::vector<{int16,char,bool}>,test_ns::OwnType}}>}>},std::set<char>}";

    EXPECT_EQ( signature<T4>(), sign );
    EXPECT_EQ( signature_crc<T4>(), calc_crc(sign) );

    //  Checked with online calculator.
    EXPECT_EQ( signature_crc<T4>(), 0x89EA5EEC );
}
//=======================================================================================




