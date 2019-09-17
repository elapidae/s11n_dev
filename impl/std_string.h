#ifndef S11N_IMPL_STD_STRING_H
#define S11N_IMPL_STD_STRING_H

#include <string>

#include "impl/size.h"
#include "impl/encode.h"
#include "impl/decode.h"
#include "impl/size.h"

//=======================================================================================
//  По умолчанию перегружен Serial<std::string>, иначе выведется истинный тип строки:
//  std::__cxx11::basic_string<char>
//=======================================================================================
namespace s11n
{
    template<typename> struct Serial;

    template<>
    struct Serial< std::string >
    {
        static constexpr auto name_of_type = "std::string";

        static void write( const std::string& str, impl::Writer* writer )
        {
            using namespace s11n::impl;
            encode( Size(str.size()), writer );
            writer->write_str( str );
        }

        static std::string read( impl::Reader* reader )
        {
            using namespace s11n::impl;
            auto size = decode<Size>( reader );
            return reader->read_str( size );
        }
    }; // Serial< std::string >
} // s11n namespace
//=======================================================================================

#endif // S11N_IMPL_STD_STRING_H
