#ifndef TEST_METAARGS_H
#define TEST_METAARGS_H

#include "tests_preparing.h"

using namespace std;
using namespace s11n;


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
    using MA3 = MetaArgs<bool,char,double>;
    EXPECT_EQ( signature_crc<MA0>(), calc_crc("MetaArgs<>") );
    EXPECT_EQ( signature_crc<MA1>(), calc_crc("MetaArgs<int32>") );
    EXPECT_EQ( signature_crc<MA3>(), calc_crc("MetaArgs<bool,char,float64>") );
}
//=======================================================================================




#endif // TEST_METAARGS_H
