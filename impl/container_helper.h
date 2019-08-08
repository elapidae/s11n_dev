#pragma once

#include <type_traits>

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



} // namespace impl
} // namespace s11n

