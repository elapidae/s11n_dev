#ifndef TUPLE_HELPER_H
#define TUPLE_HELPER_H

#include <tuple>
#include <type_traits>



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
namespace s11n {
namespace impl
{
    //===================================================================================
    template<typename T> constexpr
    bool is_tuple();
    //===================================================================================
    template <typename T>
    constexpr bool has_serial_tuple();
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
#ifndef void_t
    namespace std
    {
        template< class... >
        using void_t = void;
    }
#endif

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
    template <typename T>
    constexpr bool has_serial_tuple()
    {
        return _has_serial_tuple<T>::value;
    }
    //===================================================================================
} // namespace impl
} // namespace s11n
//=======================================================================================

#endif // TUPLE_HELPER_H
