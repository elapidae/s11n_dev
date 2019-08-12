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

#include "test_s11n/test_name_of_type.cpp"

#include <iostream>
#define vdeb std::cout << __FILE__ << ":" << __LINE__

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
TEST_F( Test_s11n, ariphmetic_name_of_type )
{
    EXPECT_EQ( s11n::name_of_type<bool>(),     "bool" );
    EXPECT_EQ( s11n::name_of_type<char>(),     "char" );

    EXPECT_EQ( s11n::name_of_type<int8_t>(),   "int8"  );
    EXPECT_EQ( s11n::name_of_type<int16_t>(),  "int16" );
    EXPECT_EQ( s11n::name_of_type<int32_t>(),  "int32" );
    EXPECT_EQ( s11n::name_of_type<int64_t>(),  "int64" );

    EXPECT_EQ( s11n::name_of_type<uint8_t>(),  "uint8"  );
    EXPECT_EQ( s11n::name_of_type<uint16_t>(), "uint16" );
    EXPECT_EQ( s11n::name_of_type<uint32_t>(), "uint32" );
    EXPECT_EQ( s11n::name_of_type<uint64_t>(), "uint64" );

    EXPECT_EQ( s11n::name_of_type<float>(),    "float32" );
    EXPECT_EQ( s11n::name_of_type<double>(),   "float64" );
}
//=======================================================================================


//=======================================================================================
struct OwnType {};
TEST_F( Test_s11n, own_name_of_type )
{
    EXPECT_EQ( s11n::name_of_type<OwnType>(), "OwnType"  );
    EXPECT_NE( s11n::name_of_type<OwnType>(), "_OwnType" );
}
//=======================================================================================


//=======================================================================================
TEST_F( Test_s11n, string_name_of_type )
{
    //  По умолчанию перегружен Serial<std::string>, иначе выведется истинный тип строки:
    //  std::__cxx11::basic_string<char>

    EXPECT_EQ( s11n::name_of_type<std::string>(), "std::string" );
    EXPECT_NE( s11n::name_of_type<OwnType>(), "std::__cxx11::basic_string<char>" );
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

TEST_F( Test_s11n, Redefined_name_of_type_name_of_type )
{
    EXPECT_EQ( s11n::name_of_type<Redefined_name_of_type>(),
               "My redefined name of type" );
}
//=======================================================================================
