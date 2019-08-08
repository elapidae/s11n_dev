#pragma once

#include <type_traits>
#include <utility>  // for std::forward

namespace s11n {
namespace impl
{

    template<typename C, typename = void>
    struct _is_container
        : public std::false_type
    {};

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

    template<typename C> constexpr
    bool is_container()
    {
        return _is_container<C>::value;
    }

    template<typename Cont, typename Value>
    void container_append( Cont* c, Value && val )
    {
        c->push_back( std::forward<Value>(val) );
    }

} // namespace impl
} // namespace s11n

