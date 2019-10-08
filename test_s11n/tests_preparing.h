#ifndef TEST_MAIN_H
#define TEST_MAIN_H

//  All includes here, for any tests.

#include "gtest/gtest.h"

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

#include "s11n.h"


// For pretty printing.
#include <iostream>
#define vdeb std::cout << __FILE__ << ":" << __LINE__ << "==> "


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wweak-vtables"
class Test_s11n: public testing::Test {};
#pragma GCC diagnostic pop


//  GTest macros give this warning. Kill it for all tests...
#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"

//=======================================================================================
template<typename T>
static s11n::crc_type own_crc()
{
    return s11n::calc_crc( s11n::signature<T>() );
}
//=======================================================================================


#endif //  TEST_MAIN_H
