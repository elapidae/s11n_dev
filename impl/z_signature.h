#ifndef SIGNATURE_H
#define SIGNATURE_H

#include <string>
#include "impl/str_view.h"
#include "impl/name_of_type_from_pf.h"

namespace s11n {

template<typename T> struct Serial;

namespace impl {


//=======================================================================================
//      Выяснение есть ли у типа инстанция Serial<T> и наличие в ней name_of_type.

template<typename T>
using _serial_name_of_type = decltype(s11n::Serial<T>::name_of_type);

template<typename T>
static constexpr bool _is_serial_name_of_type()
{
    return std::is_same
    <
        _serial_name_of_type<T>,
        _serial_name_of_type<T>
    >::value;
}


template<typename T, typename = void>
struct _has_serial_name_of_type: std::false_type {};

template<typename T>
struct _has_serial_name_of_type
        <
            T,
            typename std::void_t<decltype(s11n::Serial<T>::name_of_type)>
        >
    : std::true_type
{};

template<typename T> static constexpr
bool has_serial_name_of_type()
{
    return _has_serial_name_of_type<T>::value;
}

template <typename T>
static constexpr typename
std::enable_if
<
    has_serial_name_of_type<T>(),
    str_view
>::type
name_of_type()
{
    return { s11n::Serial<T>::name_of_type,
             _index_of(s11n::Serial<T>::name_of_type,'\0') };
}

//template<typename T> static constexpr
//bool has_serial_name_of_type_v = _has_serial_name_of_type<T>::value;




} // namespace impl
} // namespace s11n

#endif // SIGNATURE_H
