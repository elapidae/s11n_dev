#ifndef LOAD_SAVE_H
#define LOAD_SAVE_H

#include <type_traits>
#include <string>


namespace s11n {
namespace impl {


static constexpr char _true  = 0x11;
static constexpr char _false = 0xEE;

//=======================================================================================
static_assert( BYTE_ORDER == BIG_ENDIAN || BYTE_ORDER == LITTLE_ENDIAN,
               "Unknown byte order" );
//=======================================================================================
template <typename T>
static typename std::enable_if< std::is_arithmetic<T>::value,T>::type
to_little_endian( T val )
{
  #if BYTE_ORDER == LITTLE_ENDIAN
    return val;
  #else

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
  #endif
}

class Load
{
    template<typename T> std::enable_if< std::is_same<bool,T>::value, void >
    push(T b)
    {
        res.push_back( b ? _true : _false );
    }

    template<typename T> std::enable_if< std::is_arithmetic<T>::value, void >
    push(T a)
    {
        a = to_little_endian(a);
        auto *ch = static_cast<char*>( static_cast<void*>(&a) );
        res.append( ch, sizeof(a) );
    }

    std::string res;

};

class Save
{
    template<typename T> std::enable_if< std::is_same<bool,T>::value, void >
    operator ()(T b)
    {
        res.push_back( b ? _true : _false );
    }

    template<typename T> std::enable_if< std::is_arithmetic<T>::value, void >
    operator ()(T a)
    {
        a = to_little_endian(a);
        auto *ch = static_cast<char*>( static_cast<void*>(&a) );
        res.append( ch, sizeof(a) );
    }

    std::string res;
};

} // namespace impl
} // namespace s11n

#endif // LOAD_SAVE_H
