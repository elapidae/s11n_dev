#include "tests_preparing.h"

using namespace std;
using namespace s11n;

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

