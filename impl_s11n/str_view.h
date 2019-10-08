#ifndef S11N_IMPL_STR_VIEW
#define S11N_IMPL_STR_VIEW

#include <string>

//=======================================================================================
namespace s11n {
namespace impl_s11n
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
    //  Индекс первого найденного символа в строке. Здесь вход в рекурсию.
    constexpr size_t str_index_of( const char *src, char pattern );
    //===================================================================================
    //  length -- for comfort.
    constexpr size_t str_length( const char *src );
    //===================================================================================
}} // namespace s11n::impl_s11n
//=======================================================================================


//=======================================================================================
//      Implementation
//=======================================================================================
namespace s11n {
namespace impl_s11n
{
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
    constexpr size_t str_index_of( const char *src, char pattern )
    {
        return _index_of_with_recurse( src, pattern, 0 );
    }
    //===================================================================================
    //  length -- for comfort.
    constexpr size_t str_length( const char *src )
    {
        return str_index_of( src, '\0' );
    }
    //===================================================================================
}} // namespace s11n::impl_s11n
//=======================================================================================

#endif // S11N_IMPL_STR_VIEW
