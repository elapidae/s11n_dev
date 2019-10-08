#ifndef TEST_NAME_OF_TYPE_H
#define TEST_NAME_OF_TYPE_H

#include "tests_preparing.h"

using namespace std;
using namespace s11n;


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


#endif // TEST_NAME_OF_TYPE_H
