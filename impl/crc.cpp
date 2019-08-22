#include "impl/crc.h"

//=======================================================================================
namespace s11n {
namespace impl {
    //===================================================================================
    crc_type calc_crc( const std::string& buf )
    {
        uint32_t crc = crc_first_val;
        for ( auto ch: buf )
        {
            uint8_t uch  = static_cast<uint8_t>(ch);
            uint8_t ucrc = static_cast<uint8_t>(crc);
            uint8_t idx = ucrc ^ uch;
            crc = (crc >> 8) ^ _poly_04C11DB7_table[idx];
        }
        crc ^= crc_last_xor;
        return crc;
    }
    //===================================================================================
}} // s11n::impl namespaces
//=======================================================================================
