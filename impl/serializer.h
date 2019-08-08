#ifndef SERIALIZER_H
#define SERIALIZER_H


#include "impl/endian.h"

#include <type_traits>
#include <stdexcept>

#include <string.h> // for memcpy

namespace s11n {
namespace impl {


//=======================================================================================
//static constexpr char _bool_as_char( bool b )
//{
//    return b ? char(0x11) : char(0xEE);
//}
////=======================================================================================
//static bool _char_as_bool( char c )
//{
//    return c == char(0x11) ? true :
//           c == char(0xEE) ? false
//                           : (throw std::runtime_error("bool not correct"), false);
//}
//=======================================================================================





class Serializer
{
public:

    template<typename T>
    typename std::enable_if< std::is_arithmetic<T>::value, void>::type
    put(T val)
    {
        val = little_endian(val);
        auto *ch = static_cast<char*>( static_cast<void*>(&val) );
        res.append( ch, sizeof(val) );
    }

    void put( const std::string& val )
    {
        res += val;
    }

    std::string res;
};


class Deserializer
{
public:
    Deserializer( const std::string& src );

    template<typename T>
    typename std::enable_if< std::is_arithmetic<T>::value, T >::type
    get()
    {
        static_assert ( sizeof(bool) == 1, "strange bool size" );
        return _get<T>();
    }

    std::string get_str( uint32_t sz )
    {
        if ( _remained < sz )
            throw std::out_of_range("Deserializer::get_str()");

        auto res = std::string( _ptr, sz );

        _remained -= sz;
        _ptr += sz;

        return res;
    }

private:

    template<typename T>
    T _get()
    {
        if ( _remained < sizeof(T) )
            throw std::out_of_range("Deserializer::_get()");

        //  If use as cast, can be problems with alignment :(((
        //T res = *static_cast<const T*>( static_cast<const void*>(_pos) );

        T res;
        auto res_ptr = static_cast<void*>( &res );
        memcpy( res_ptr, _ptr, sizeof(T) );

        _remained -= sizeof(T);
        _ptr += sizeof(T);

        return little_endian( res );
    }


    std::string _src;
    const char *_ptr;
    size_t _remained;
};


} // namespace impl
} // namespace s11n

#endif // SERIALIZER_H
