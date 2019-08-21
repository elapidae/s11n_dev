#pragma once

#include <string>


//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    struct str_view
    {
        const char *ptr;
        size_t len;

        std::string str() const
        {
            return {ptr, len};
        }
        operator std::string() const { return str(); }
    };
    //===================================================================================
    std::ostream& operator << ( std::ostream& os, const str_view& sv );
    //===================================================================================

    //===================================================================================
    //  Индекс первого найденного символа в строке, в рекурсии.
    //  См. _index_of()
    static constexpr size_t _index_of_with_recurse( const char *src,
                                                    char pattern,
                                                    size_t accum )
    {
        //  cannot in c++11: static_assert ( *src != '\0', "" );
        return *src == pattern
                    ? accum
                    : _index_of_with_recurse( src + 1, pattern, accum + 1 );
    }
    //===================================================================================
    //  Индекс первого найденного символа в строке. Здесь вход в рекурсию.
    static constexpr size_t str_index_of( const char *src, char pattern )
    {
        return _index_of_with_recurse( src, pattern, 0 );
    }
    //===================================================================================
    //  length -- for comfort.
    static constexpr size_t str_length( const char *src )
    {
        return str_index_of( src, '\0' );
    }
    //===================================================================================
} // namespace impl
} // namespace s11n
//=======================================================================================
