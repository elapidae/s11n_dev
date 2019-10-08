
#include "tests_preparing.h"

using namespace std;
using namespace s11n;


//=======================================================================================
using Tup0 = tuple<>;
using Tup1b = tuple<bool>;
using Tup1u64 = tuple<uint64_t>;

template <typename ...>
struct MetaArgs2 {};

using TupT = tuple<Tup0,Tup1b,Tup1u64,char,bool,uint16_t,
                   MetaArgs2<Tup1b>>;

TEST_F( Test_s11n, tuple_names )
{
    EXPECT_EQ( signature<Tup0>(), "{}" );
    EXPECT_EQ( signature_crc<Tup0>(), calc_crc("{}") );

    EXPECT_EQ( signature<Tup1b>(), "{bool}" );
    EXPECT_EQ( signature_crc<Tup1b>(), calc_crc("{bool}") );

    EXPECT_EQ( signature<Tup1u64>(), "{uint64}" );
    EXPECT_EQ( signature_crc<Tup1u64>(), calc_crc("{uint64}") );

    string sign_TT = "{{},{bool},{uint64},char,bool,uint16,"
                      "MetaArgs2<{bool}>"
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
namespace test_ns
{
    struct LulzType {};
}

TEST_F( Test_s11n, some_random_signature )
{

    using T = tuple<int16_t, char, bool>;
    using V = vector<T>;
    using T2 = tuple<T,V,test_ns::LulzType>;
    using C = map<int,T2>;
    using UM = unordered_map<int,C>;
    using T3 = tuple<C,T2,V,T,UM>;
    using S = set<char>;
    using T4 = tuple<T,V,T2,C,UM,T3,S>;


    EXPECT_EQ( signature<T>(),  "{int16,char,bool}" );
    EXPECT_EQ( signature<V>(),  "std::vector<{int16,char,bool}>" );
    EXPECT_EQ( signature<T2>(), "{{int16,char,bool},std::vector<{int16,char,bool}>,"
                                "test_ns::LulzType}" );


    auto sign = "{{int16,char,bool},std::vector<{int16,char,bool}>,"
                "{{int16,char,bool},std::vector<{int16,char,bool}>,"
                "test_ns::LulzType},std::map<{int32,{{int16,char,bool},"
                "std::vector<{int16,char,bool}>,test_ns::LulzType}}>,"
                "std::unordered_map<{int32,std::map<{int32,{{int16,char,bool},"
                "std::vector<{int16,char,bool}>,test_ns::LulzType}}>}>,"
                "{std::map<{int32,{{int16,char,bool},std::vector<{int16,char,bool}>,"
                "test_ns::LulzType}}>,{{int16,char,bool},std::vector<{int16,char,bool}>,"
                "test_ns::LulzType},std::vector<{int16,char,bool}>,"
                "{int16,char,bool},"
                "std::unordered_map<{int32,std::map<{int32,{{int16,char,bool},"
                "std::vector<{int16,char,bool}>,test_ns::LulzType}}>}>},std::set<char>}";

    EXPECT_EQ( signature<T4>(), sign );
    EXPECT_EQ( signature_crc<T4>(), calc_crc(sign) );

    //  Checked with online calculator.
    EXPECT_EQ( signature_crc<T4>(), 0x7477564A );
}
//=======================================================================================
