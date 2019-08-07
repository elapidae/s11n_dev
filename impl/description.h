#ifndef DESCRIPTION_H
#define DESCRIPTION_H

#include "impl/str_view.h"
#include <array>

//=======================================================================================
namespace s11n
{
    template <typename T>
    struct Serial;
} // namespace s11n
//=======================================================================================



//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    //      Serial<T>::description deduction
    //
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
    template<typename T> static constexpr
    bool has_serial_description()
    {
        return _has_serial_description<T>::value;
    }
    //-----------------------------------------------------------------------------------
    template <typename T>
    static constexpr typename
    std::enable_if
    <
        has_serial_description<T>(),
        impl::str_view
    >::type
    description()
    {
        return
        {
            s11n::Serial<T>::description,
            length( s11n::Serial<T>::description )
        };
    }
    //===================================================================================

    //===================================================================================
    //      Elsewhere description is empty
    //
    template <typename T>
    static constexpr typename
    std::enable_if
    <
        !has_serial_description<T>(),
        impl::str_view
    >::type
    description()
    {
        return {"",0};
    }
    //===================================================================================

    //===================================================================================
    template <typename T>
    static constexpr
    std::array<impl::str_view,3> description_in_squares()
    {
        using sv = impl::str_view;
        return description<T>().len == 0
                ? std::array<sv,3>{ sv{"",0}, sv{"",0}, sv{"",0} }
                : std::array<sv,3>{ sv{"(",1}, description<T>(), sv{")",1} };
    }
    //===================================================================================
    template <typename T>
    static std::string description_in_squares_str()
    {
        auto d = description_in_squares<T>();
        return d[0].str() + d[1].str() + d[2].str();
    }
    //===================================================================================
    //      debug only, need to kill...
    std::ostream& operator << (std::ostream& os, const std::array<str_view,3>& ar);
    //===================================================================================

} // namespace impl
} // namespace s11n
//=======================================================================================

#endif // DESCRIPTION_H
