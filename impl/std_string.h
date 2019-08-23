#pragma once

#include <string>

#include <s11n_size.h>
//#include <s11n_decoder.h>
//#include <s11n_encoder.h>

//=======================================================================================
//  По умолчанию перегружен Serial<std::string>, иначе выведется истинный тип строки:
//  std::__cxx11::basic_string<char>
//=======================================================================================
namespace s11n
{
    template<typename> struct Serial;
    class Encoder;
    class AbstractDecoder;

    template<>
    struct Serial<std::string>
    {
        static constexpr auto name_of_type = "std::string";

//        static void write( const std::string& str, Writer* writer )
//        {
//            s11n::Encoder::encode( s11n::Size(str.size()), writer );
//            writer->write_str( str );
//        }

//        static std::string read( Reader* reader )
//        {
//            auto size = AbstractDecoder::decode<s11n::Size>( reader );
//            return reader->read_str( size );
//        }
    };
};
//=======================================================================================
