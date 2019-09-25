
#include "test_main.h"

using namespace std;
using namespace s11n;

//=======================================================================================
template<typename T>
static crc_type own_crc()
{
    return calc_crc( signature<T>() );
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
    static_assert ( is_same<std::string::value_type,char>::value, "" );
    EXPECT_EQ( signature< string >(), "std::string<char>" );

    EXPECT_EQ( signature< vector<bool>              >(), "std::vector<bool>"        );
    EXPECT_EQ( signature< deque<bool>               >(), "std::deque<bool>"         );
    EXPECT_EQ( signature< valarray<bool>            >(), "std::valarray<bool>"      );
    EXPECT_EQ( signature< unordered_set<bool>       >(), "std::unordered_set<bool>" );
    EXPECT_EQ( signature< list<char>                >(), "std::list<char>"          );
    EXPECT_EQ( signature< set<bool>                 >(), "std::set<bool>"           );
    EXPECT_EQ( signature< multiset<char>            >(), "std::multiset<char>"      );
    EXPECT_EQ( signature< unordered_set<char>       >(), "std::unordered_set<char>" );

    EXPECT_EQ( signature< unordered_multiset<char>  >(),
                    "std::unordered_multiset<char>" );

    // Надо делать using, иначе ломается макрос сравнения.
    using AT   = array<bool,13>;

    using MT   = map<int8_t,bool>;
    using MMT  = multimap<int8_t,bool>;

    using UMT  = unordered_map<int8_t,bool>;
    using UMMT = unordered_multimap<int8_t,bool>;


    EXPECT_EQ( signature< AT   >(), "std::array<bool>"                      );
    EXPECT_EQ( signature< MT   >(), "std::map<{int8,bool}>"                 );
    EXPECT_EQ( signature< MMT  >(), "std::multimap<{int8,bool}>"            );
    EXPECT_EQ( signature< UMT  >(), "std::unordered_map<{int8,bool}>"       );
    EXPECT_EQ( signature< UMMT >(), "std::unordered_multimap<{int8,bool}>"  );

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


    EXPECT_EQ( signature<T>(),  "{int16,char,bool}" );
    EXPECT_EQ( signature<V>(),  "std::vector<{int16,char,bool}>" );
    EXPECT_EQ( signature<T2>(), "{{int16,char,bool},std::vector<"
                                 "{int16,char,bool}>,test_ns::OwnType}" );


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
