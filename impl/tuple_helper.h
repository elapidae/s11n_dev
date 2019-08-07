#ifndef TUPLE_HELPER_H
#define TUPLE_HELPER_H

#include <tuple>

namespace s11n { template <typename> struct Serial; }


//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    //      is_tuple<T>() use as marker with signatures.
    //
    template<typename T, typename = void>
    struct _has_tuple_size
        : std::false_type
    {};
    //-----------------------------------------------------------------------------------
    template<typename T>
    struct _has_tuple_size< T, std::void_t<decltype(std::tuple_size<T>::value)> >
        : std::true_type
    {};
    //-----------------------------------------------------------------------------------
    template<typename T> static constexpr
    bool is_tuple()     { return _has_tuple_size<T>::value; }
    //===================================================================================


    //===================================================================================
    //      for simplify...
    //
    template<typename T> static constexpr
    int tuple_size() { return std::tuple_size<T>::value; }
    //-----------------------------------------------------------------------------------
    template<typename T, int idx>
    using tuple_element = typename std::tuple_element<idx,T>::type;
    //===================================================================================


    //===================================================================================
    //      Use for recurse tuple unpacking.
    //      Используется при итерациях с кортежами.
    //
    template<typename T> static constexpr
    int tuple_start_idx()   { return std::tuple_size<T>::value == 0 ? -1 : 0; }
    //-----------------------------------------------------------------------------------
    template<int idx, typename T> static constexpr
    int tuple_next_idx()    { return idx + 1 == tuple_size<T>() ? -1 : idx + 1; }
    //===================================================================================


    //===================================================================================
    template <typename T, typename = void>
    struct _has_serial_tuple
        : std::false_type
    {};
    //-----------------------------------------------------------------------------------
    template <typename T>
    struct _has_serial_tuple
    <
        T,
        std::void_t< decltype(s11n::Serial<T>::to_tuple) >
    >
        : std::true_type
    {};
    //-----------------------------------------------------------------------------------
    template <typename T>
    static constexpr bool has_serial_tuple()
    {
        return _has_serial_tuple<T>::value;
    }
    //===================================================================================
} // namespace impl
} // namespace s11n
//=======================================================================================

#endif // TUPLE_HELPER_H
