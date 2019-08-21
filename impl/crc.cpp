#include "impl/crc.h"

namespace s11n {
namespace impl {
    uint32_t poly_04C11DB7( const std::string &buf )
    {
        uint32_t crc = 0xFFFFFFFF;
        for ( auto ch: buf )
        {
            uint8_t uch  = static_cast<uint8_t>(ch);
            uint8_t ucrc = static_cast<uint8_t>(crc);
            uint8_t idx = ucrc ^ uch;
            crc = (crc >> 8) ^ _poly_04C11DB7_table[idx];
        }
        crc ^= 0xFFFFFFFF;
        return crc;
    }
}
}
