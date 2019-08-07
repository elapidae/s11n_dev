#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <endian.h>
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


//=======================================================================================
//  Функция выворачивает число наизнанку, если архитектура big_endian.
template <typename T>
static typename std::enable_if< std::is_arithmetic<T>::value,T>::type
to_little_endian( T val )
{
    static_assert( sizeof(T) <= 8 && (sizeof(T) == 1 || sizeof(T) % 2 == 0),
                   "Strange size of T, must be 1 or 2 or 4 or 8." );

  #if BYTE_ORDER == LITTLE_ENDIAN
    // nothing to do...
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
  #else
    #error "Unknown byte order";
  #endif
    return val;
}



class Serializer
{
public:

    template<typename T>
    typename std::enable_if< std::is_arithmetic<T>::value, void>::type
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
        static_assert ( sizeof(bool) == 1, "strange bool size" );
        return _get<T>();
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

        return to_little_endian( res );
    }


    std::string _src;
    const char *_ptr;
    size_t _remained;
};


} // namespace impl
} // namespace s11n

#endif // SERIALIZER_H
