#pragma once


#include <type_traits>
#include <array>
#include "impl/str_view.h"


#ifndef void_t
    namespace std
    {
        template< class... >
        using void_t = void;
    }
#endif

//=======================================================================================
//      description: введена с целью ведения версионности, изменений типов.
//
//  Если в перегрузке есть соотвествующее поле:
//  template <>
//  struct s11n::Serial<SomeType>
//  {
//      static constexpr auto description = "some words, ver. 1";
//  };
//  , то в сигнатуру типа описание войдет в скобочках: "SomeType(some words, ver. 1)".
//  Если такого поля нету, то описания не будет.
//=======================================================================================
namespace s11n {
namespace impl
{
    template <typename T> constexpr
    impl::str_view description();

    template <typename T> constexpr
    std::array<impl::str_view,3> description_in_squares();

    template <typename T>
    std::string description_in_squares_str();

}} // s11n::impl namespaces
//=======================================================================================



//=======================================================================================
//      Implementation
//=======================================================================================
//      Need forward declaration
namespace s11n
{
    template <typename T> struct Serial;
}
//=======================================================================================

namespace s11n {
namespace impl
{
    //===================================================================================
    //      Serial<T>::description deduction
    template<typename T, typename = void>
    struct _has_serial_description
        : std::false_type
    {};
    //-----------------------------------------------------------------------------------
    template<typename T>
    struct _has_serial_description
            <
                T,
                typename std::void_t< decltype(s11n::Serial<T>::description) >
            >
        : std::true_type
    {};
    //-----------------------------------------------------------------------------------
    template<typename T> constexpr
    bool has_serial_description()
    {
        return _has_serial_description<T>::value;
    }
    //===================================================================================


    //===================================================================================
    //
    //  Без if constexpr, в 11-ом свернуть нельзя.
    template <typename T> constexpr typename
    std::enable_if< has_serial_description<T>(), impl::str_view>::type
    _description()
    {
        return
        {
            s11n::Serial<T>::description,
            length( s11n::Serial<T>::description )
        };
    }
    //===================================================================================
    template <typename T> constexpr typename
    std::enable_if< !has_serial_description<T>(), impl::str_view >::type
    _description()
    {
        return {};
    }
    //===================================================================================
    template <typename T>
    constexpr impl::str_view description()
    {
        return _description<T>();
    }
    //===================================================================================

    //===================================================================================
    template <typename T> constexpr
    std::array<impl::str_view,3> description_in_squares()
    {
        using sv = impl::str_view;
        return description<T>().len == 0
                ? std::array<sv,3>{ sv{}, sv{}, sv{} }
                : std::array<sv,3>{ sv{"(",1}, description<T>(), sv{")",1} };
    }
    //===================================================================================
    template <typename T>
    std::string description_in_squares_str()
    {
        auto d = description_in_squares<T>();
        return d[0].str() + d[1].str() + d[2].str();
    }
    //===================================================================================
} // namespace impl
} // namespace s11n
//=======================================================================================
