#ifndef S11N_IMPL_TUPLE_HELPER_H
#define S11N_IMPL_TUPLE_HELPER_H


#include <tuple>
#include <type_traits>
#include "impl/std_void_t.h"


//=======================================================================================
//      serial tuple
//
//      Чтобы можно было сериализовывать и десериализовывать объекты, был выбран путь
//  соспоставления кортежа с каким-либо типом. Для бысторого внедрения типа достаточно
//  специализировать структуру s11n::Serial, в которой создать статический метод
//  std::tuple<args..> to_tuple(Type);
//
//  Например:
//  namespace s11n
//  {
//      template<>
//      struct Serial<std::chrono::seconds>
//      {
//          static std::tuple<int64_t> to_tuple( const std::chrono::seconds& sec )
//          {
//              return std::make_tuple( sec.count() );
//          }
//      };
//  }
//
//  Порядок аргументов достжен соответствовать хотя бы одному конструктору объекта, тогда
//  декодер сможет инстанцировать объект.
//
//=======================================================================================
namespace s11n { template<typename> struct Serial; }

namespace s11n {
namespace impl
{
    //===================================================================================
    template<typename T> constexpr
    bool is_tuple();
    //===================================================================================
    template<typename T> constexpr
    bool has_serial_tuple();
    //===================================================================================
    template<typename T>
    using serial_tuple_type = decltype(s11n::Serial<T>::to_tuple(std::declval<T>()));
    //===================================================================================

    //===================================================================================
    template<typename T> constexpr
    int tuple_size();
    //-----------------------------------------------------------------------------------
    template<typename T, int idx>
    using tuple_element = typename std::tuple_element<idx,T>::type;
    //-----------------------------------------------------------------------------------
    template<typename T> constexpr
    int tuple_start_idx();
    //-----------------------------------------------------------------------------------
    template<int idx, typename T> constexpr
    int tuple_next_idx();
    //===================================================================================
}}
//=======================================================================================


//=======================================================================================
//      Implementation
//=======================================================================================
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
    template<typename T> constexpr
    bool is_tuple() { return _has_tuple_size<T>::value; }
    //===================================================================================


    //===================================================================================
    //      for simplify...
    //
    template<typename T> constexpr
    int tuple_size() { return std::tuple_size<T>::value; }
    //===================================================================================


    //===================================================================================
    //      Use for recurse tuple unpacking.
    //      Используется при итерациях с кортежами.
    //
    template<typename T> constexpr
    int tuple_start_idx() { return std::tuple_size<T>::value == 0 ? -1 : 0; }
    //-----------------------------------------------------------------------------------
    template<int idx, typename T> constexpr
    int tuple_next_idx()  { return idx + 1 == tuple_size<T>() ? -1 : idx + 1; }
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
    //  Проверка, что to_tuple() возвращает какой-то кортеж.
    template<typename T> constexpr typename std::enable_if<
    _has_serial_tuple<T>::value, bool>::type
    _serial_method_is_tuple()
    {
        return is_tuple<serial_tuple_type<T>>();
    }

    template<typename T> constexpr typename std::enable_if<
    !_has_serial_tuple<T>::value, bool>::type
    _serial_method_is_tuple()
    {
        return false;
    }

    template <typename T>
    constexpr bool has_serial_tuple()
    {
        static_assert( !_has_serial_tuple<T>::value ||
                       _serial_method_is_tuple<T>(),
                       "Serial<T>::to_tuple() must return std::tuple only." );

        return _has_serial_tuple<T>::value;
    }
    //===================================================================================
} // namespace impl
} // namespace s11n
//=======================================================================================

#endif // S11N_IMPL_TUPLE_HELPER_H
