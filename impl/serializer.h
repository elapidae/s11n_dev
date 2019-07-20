#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <endian.h>
#include <type_traits>
#include <stdexcept>

namespace s11n {
namespace impl {


//=======================================================================================
static constexpr char _bool_as_char( bool b )
{
    return b ? char(0x11) : char(0xEE);
}
//=======================================================================================
static bool _char_as_bool( char c )
{
    return c == char(0x11) ? true :
           c == char(0xEE) ? false
                           : (throw std::runtime_error("bool not correct"), false);
}
//=======================================================================================


//=======================================================================================
//  Функция выворачивает число наизнанку, если архитектура big_endian.
template <typename T>
static typename std::enable_if< std::is_arithmetic<T>::value,T>::type
to_little_endian( T val )
{
  #if BYTE_ORDER == LITTLE_ENDIAN
    return val;
  #elif BYTE_ORDER == BIG_ENDIAN
    auto *ch = static_cast<char*>( static_cast<void*>(&val) );
    constexpr auto tsize = sizeof(T);

    switch( tsize )
    {
    case 8: std::swap( ch[3], ch[tsize-4] );
            std::swap( ch[2], ch[tsize-3] );
    case 4: std::swap( ch[1], ch[tsize-2] );
    case 2: std::swap( ch[0], ch[tsize-1] );
    }

    return val;
  #else
    #error "Unknown byte order";
  #endif
}



class Serializer
{
public:
    template<typename T>
    typename std::enable_if< std::is_same<T,bool>::value, void>::type
    put(T val)
    {
        res.push_back( _bool_as_char(val) );
    }


    template<typename T>
    typename std::enable_if< std::is_arithmetic<T>::value &&
                            !std::is_same<T,bool>::value, void>::type
    put(T val)
    {
        val = to_little_endian(val);
        auto *ch = static_cast<char*>( static_cast<void*>(&val) );
        res.append( ch, sizeof(val) );
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
        auto res = _get<T>();

        if ( std::is_same<T,bool>::value )
            return _char_as_bool( res );

        return res;
    }


private:

    template<typename T>
    T _get()
    {
        if ( _remained < sizeof(T) )
            throw std::out_of_range("Deserializer::_get()");

        T res = *static_cast<const T*>( static_cast<const void*>(_pos) );
        _remained -= sizeof(T);
        _pos += sizeof(T);

        return to_little_endian( res );
    }


    std::string _src;
    const char *_pos;
    size_t _remained;
};


} // namespace impl
} // namespace s11n

#endif // SERIALIZER_H
