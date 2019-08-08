#ifndef ENDIAN_H
#define ENDIAN_H

#include <type_traits>
#include <endian.h>

namespace s11n {
namespace impl {

    //===================================================================================
    //  Функция выворачивает число наизнанку, если архитектура big_endian.
    template <typename T> typename std::enable_if<std::is_arithmetic<T>::value,T>::type
    little_endian( T val )
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
    //===================================================================================


} // namespace impl
} // namespace s11n

#endif // ENDIAN_H
