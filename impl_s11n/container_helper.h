#ifndef S11N_IMPL_CONTAINER_HELPER
#define S11N_IMPL_CONTAINER_HELPER

#include <utility>      // for std::forward
#include <type_traits>

#include "impl_s11n/void_type.h"

//=======================================================================================
namespace s11n {
namespace impl_s11n
{
    //===================================================================================
    template<typename C> constexpr
    bool is_container();
    //===================================================================================
    template<typename C> constexpr
    bool is_map_set();
    //===================================================================================
    template<typename Cont, typename Value>
    typename std::enable_if<!is_map_set<Cont>(), void>::type
    container_append( Cont* c, Value && val );
    //-----------------------------------------------------------------------------------
    template<typename Cont, typename Value>
    typename std::enable_if<is_map_set<Cont>(), void>::type
    container_append( Cont* c, Value && val );
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
    template<typename C, typename = void>
    struct _is_container
        : public std::false_type
    {};
    //-----------------------------------------------------------------------------------
    template<typename C>
    struct _is_container
    <
        C,
        void_type
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

    //===================================================================================
    template<typename C, typename = void>
    struct _is_map_set
        : public std::false_type
    {};
    //-----------------------------------------------------------------------------------
    template<typename C>
    struct _is_map_set
    <
        C,
        void_type
        <
            typename C::value_type,
            typename C::key_type,
            decltype( std::declval<C>().insert
                        (
                            *static_cast<typename C::value_type*>(nullptr)
                        )
                    )
        >
    >
        : public std::true_type
    {};
    //-----------------------------------------------------------------------------------
    template<typename C> constexpr
    bool is_map_set()
    {
        return _is_map_set<C>::value;
    }
    //===================================================================================

    //===================================================================================
    template<typename Cont, typename Value>
    typename std::enable_if<!is_map_set<Cont>(), void>::type
    container_append( Cont* c, Value && val )
    {
        c->push_back( std::forward<Value>(val) );
    }
    //===================================================================================
    template<typename Cont, typename Value>
    typename std::enable_if<is_map_set<Cont>(), void>::type
    container_append( Cont* c, Value && val )
    {
        c->insert( std::forward<Value>(val) );
    }
    //===================================================================================
}} // namespace s11n::impl_s11n
//=======================================================================================

#endif // S11N_IMPL_CONTAINER_HELPER
