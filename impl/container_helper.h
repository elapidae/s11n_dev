#ifndef S11N_IMPL_CONTAINER_HELPER
#define S11N_IMPL_CONTAINER_HELPER

#include <utility>      // for std::forward
#include <type_traits>

#include "impl/std_void_t.h"

//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    template<typename C> constexpr
    bool is_container();
    //===================================================================================
    template<typename Cont, typename Value>
    void container_append( Cont* c, Value && val );
    //===================================================================================
} // namespace impl
} // namespace s11n
//=======================================================================================


//=======================================================================================
//      Implementation
//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    template<typename C, typename = void>
    struct _is_container
        : public std::false_type
    {};
    //-----------------------------------------------------------------------------------
    template<typename C>
    struct _is_container
    <
        C,
        std::void_t
        <
            decltype(std::declval<C>().begin()),
            decltype(std::declval<C>().end()),
            decltype(std::declval<C>().size()),
            typename C::value_type
        >
    >
        : public std::true_type
    {};
    //-----------------------------------------------------------------------------------
    template<typename C> constexpr
    bool is_container()
    {
        return _is_container<C>::value;
    }
    //===================================================================================
    //  Когда дойдут руки для десериализации set-ов и map-ов, надо будет припилить через
    //  emplace()
    template<typename Cont, typename Value>
    void container_append( Cont* c, Value && val )
    {
        c->push_back( std::forward<Value>(val) );
    }
    //===================================================================================
} // namespace impl
} // namespace s11n
//=======================================================================================

#endif // S11N_IMPL_CONTAINER_HELPER
