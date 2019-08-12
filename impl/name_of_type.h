#pragma once

#include "impl/str_view.h"
#include "impl/name_of_type_from_pf.h"
#include "impl/std_string.h"

//=======================================================================================
//      name_of_type: введен для поправки имени типа, если, по какой-то причине, с этим
//  не справился самовывод. Например, если не подправить имя для std::string, то
//  имя выводится примерно так: "std::__cxx11::basic_string<char>".
//
//  NB! Работа была проверена только на компиляторе gcc!!! Для других компиляторов
//  не знаю, но если узнаю, допилю.
//
//  Чтобы поправить имя, следует определить соответствующее статическое поле:
//  template <>
//  struct s11n::Serial<std::string>
//  {
//      static constexpr auto name_of_type = "std::string";
//  };
//
//  Если поля нету, то имя типа будет выводится:
//
//   - для арифметических типов: bool, char, intN, uintN, floatN, где N - размер в битах;
//
//   - для остальных -- через парсинг результата жизнедеятельности макроса
//     __PRETTY_FUNCTION__.
//
//=======================================================================================
namespace s11n {
namespace impl
{
    template <typename T> constexpr
    impl::str_view name_of_type();
}}
//=======================================================================================



//=======================================================================================
//      Implementation
//=======================================================================================
namespace s11n
{
    template <typename T> struct Serial;
}
//=======================================================================================
//
namespace s11n {
namespace impl
{
    //===================================================================================
    //      Plain name of type deduction
    //
    template <typename T>
    constexpr const typename std::enable_if< std::is_arithmetic<T>::value,
    impl::str_view>::type _name_of_type()
    {
        static_assert( sizeof(bool)   == 1, "sizeof(bool)   != 1" );
        static_assert( sizeof(char)   == 1, "sizeof(char)   != 1" );
        static_assert( sizeof(float)  == 4, "sizeof(float)  != 4" );
        static_assert( sizeof(double) == 8, "sizeof(double) != 8" );
        static_assert( sizeof(T)      <= 8, "size is too big..."  );

        return
            std::is_same<char,T>::value     ? impl::str_view{"char", 4}    :
            std::is_same<bool,T>::value     ? impl::str_view{"bool", 4}    :

            std::is_same<int8_t,T>::value   ? impl::str_view{"int8", 4}    :
            std::is_same<int16_t,T>::value  ? impl::str_view{"int16", 5}   :
            std::is_same<int32_t,T>::value  ? impl::str_view{"int32", 5}   :
            std::is_same<int64_t,T>::value  ? impl::str_view{"int64", 5}   :

            std::is_same<uint8_t,T>::value  ? impl::str_view{"uint8", 5}   :
            std::is_same<uint16_t,T>::value ? impl::str_view{"uint16", 6}  :
            std::is_same<uint32_t,T>::value ? impl::str_view{"uint32", 6}  :
            std::is_same<uint64_t,T>::value ? impl::str_view{"uint64", 6}  :

            std::is_same<float,T>::value    ? impl::str_view{"float32", 7} :
            std::is_same<double,T>::value   ? impl::str_view{"float64", 7} :
                                              impl::str_view{"!ERROR!", 7};
    }
    //===================================================================================


    //===================================================================================
    //      Serial<T>::name_of_type deduction
    //
    template<typename T, typename = void>
    struct _has_serial_name_of_type
        : std::false_type
    {};
    //-----------------------------------------------------------------------------------
    template<typename T>
    struct _has_serial_name_of_type
            <
                T,
                typename std::void_t<decltype(s11n::Serial<T>::name_of_type)>
            >
        : std::true_type
    {};
    //-----------------------------------------------------------------------------------
    template<typename T> constexpr
    bool has_serial_name_of_type()
    {
        return _has_serial_name_of_type<T>::value;
    }
    //-----------------------------------------------------------------------------------
    template <typename T> constexpr typename
    std::enable_if
    <
        has_serial_name_of_type<T>(),
        impl::str_view
    >::type
    _name_of_type()
    {
        return
        {
            s11n::Serial<T>::name_of_type,
            length( s11n::Serial<T>::name_of_type )
        };
    }
    //===================================================================================

    //===================================================================================
    //      Elsewhere extract name_of_type from __PRETTY_FUNCTION__
    //
    template <typename T> constexpr typename
    std::enable_if
    <
        !has_serial_name_of_type<T>() && !std::is_arithmetic<T>::value,
        impl::str_view
    >::type
    _name_of_type()
    {
        return name_of_type_from_PF<T>();
    }
    //===================================================================================

    //===================================================================================
    template <typename T> constexpr
    impl::str_view name_of_type()
    {
        return _name_of_type<T>();
    }
    //===================================================================================
} // namespace impl
} // namespace s11n
//=======================================================================================
