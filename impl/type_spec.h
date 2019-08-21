#ifndef S11N_IMPL_TYPE_SPEC_H
#define S11N_IMPL_TYPE_SPEC_H

#include "impl/tuple_helper.h"
#include "impl/container_helper.h"

//=======================================================================================
//      Специализация типа нужна, чтобы специализироваться на Encoder/Decoder.
//  Действует в приоритетном порядке:
//      as_plain,
//      as_own_read_write,
//      as_serial_tuple,
//      as_container,
//      as_tuple;
//  Т.е. даже если специализировать, например, тип int, то он все равно будет считаться
//  as_plain, ибо нефиг.
//
//  Таким образом, можно специализировать свои read|write для какого-нибудь контейнера,
//  но нельзя прострелить ногу с plain типами.
//
//  Опять же, можно сделать свои read/write, но тогда нет смысла делать to_tuple().
//=======================================================================================


//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    enum class type_spec { as_plain,
                           as_own_read_write,
                           as_serial_tuple,
                           as_container,
                           as_tuple,
                           error
                          };
    //===================================================================================
    template<typename T> constexpr
    type_spec type_spec_of();
    //===================================================================================
}} // s11n::impl namespaces
//=======================================================================================


//=======================================================================================
//  Implementation:
//      _has_serial_read<T>()
//      _has_serial_write<T>()
//      has_serial_read_write<T>()
//      type_spec_of<T>()
//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    //      Has Serial<T>::write
    //===================================================================================
    template<typename SerT, typename = void>
    struct _s_has_serial_write : public std::false_type {};
    //-----------------------------------------------------------------------------------
    template<typename SerT>
    struct _s_has_serial_write
        <
            SerT,
            std::void_t
            <
                decltype(s11n::Serial<SerT>::write(std::declval<SerT>(),nullptr))
            >
        > : public std::true_type {};
    //-----------------------------------------------------------------------------------
    template<typename SerT> constexpr
    bool _has_serial_write()
    {
        return _s_has_serial_write<SerT>::value;
    }
    //===================================================================================
    //      Has Serial<T>::write
    //===================================================================================


    //===================================================================================
    //      Has Serial<T>::read
    //===================================================================================
    template<typename SerT, typename = void>
    struct _s_has_serial_read : public std::false_type {};
    //-----------------------------------------------------------------------------------
    template<typename SerT>
    struct _s_has_serial_read
        <
            SerT,
            std::void_t
            <
                decltype( s11n::Serial<SerT>::read(nullptr) )
            >
        > : public std::true_type {};
    //-----------------------------------------------------------------------------------
    template<typename SerT> constexpr
    bool _has_serial_read()
    {
        return _s_has_serial_read<SerT>::value;
    }
    //===================================================================================
    //      Has Serial<T>::read
    //===================================================================================


    //===================================================================================
    //      Has Serial<T>::read && write
    //===================================================================================
    template<typename SerT> constexpr
    bool _has_serial_read_write()
    {
        static_assert( (!_has_serial_read<SerT>() && !_has_serial_write<SerT>()) ||
                       ( _has_serial_read<SerT>() &&  _has_serial_write<SerT>()),
                        "Define both Serial<T>::read && write" );

        return _has_serial_read<SerT>() && _has_serial_write<SerT>();
    }
    //===================================================================================
    template<typename SerT> constexpr
    bool has_serial_read_write()
    {
        //  Могут быть определены либо read|write, либо to_tuple, но не одновременно.
        static_assert( ( !_has_serial_read_write<SerT>() && !has_serial_tuple<SerT>()) ||
                       (  _has_serial_read_write<SerT>() && !has_serial_tuple<SerT>()) ||
                       ( !_has_serial_read_write<SerT>() &&  has_serial_tuple<SerT>()),
                        "Define Serial<T>::read && write "
                        "or define to_tuple, but not both." );

        return _has_serial_read_write<SerT>();
    }
    //===================================================================================
    //      Has Serial<T>::read && write
    //===================================================================================


    //===================================================================================
    template<typename T> constexpr
    type_spec _type_spec_of()
    {
        return std::is_arithmetic<T>::value     ? type_spec::as_plain
             : impl::has_serial_read_write<T>() ? type_spec::as_own_read_write
             : impl::has_serial_tuple<T>()      ? type_spec::as_serial_tuple
             : impl::is_container<T>()          ? type_spec::as_container
             : impl::is_tuple<T>()              ? type_spec::as_tuple
                                                : type_spec::error;
    }
    //===================================================================================
    template<typename T> constexpr
    type_spec type_spec_of()
    {
        static_assert( _type_spec_of<T>() != type_spec::error,
                       "Cannot define spec of type" );

        return _type_spec_of<T>();
    }
    //===================================================================================
} // namespace impl
} // namespace s11n
//=======================================================================================

#endif // S11N_IMPL_TYPE_SPEC_H
