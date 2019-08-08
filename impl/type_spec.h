#ifndef TYPE_SPEC_H
#define TYPE_SPEC_H

#include "impl/tuple_helper.h"
#include "impl/container_helper.h"

namespace s11n {
namespace impl
{
    enum class type_spec { as_plain,
                           as_own_read_write,
                           as_serial_tuple,
                           as_container,
                           as_tuple,
                           error
                          };

    template<typename T> constexpr
    type_spec spec_of();


    template<typename SerT, typename = void>
    struct _has_serial_read_write : public std::false_type {};

    template<typename SerT>
    struct _has_serial_read_write
        <
            SerT,
            std::void_t
            <   // фейкрвый вызов для определения наличия write с соотв. сигнатурой.
                decltype( s11n::Serial<SerT>::write(std::declval<SerT>(),nullptr) ),
                decltype( s11n::Serial<SerT>::read(nullptr) )
            >
        >
        : public std::true_type
    {};

    template<typename SerT> constexpr
    bool has_serial_read_write()
    {
        return _has_serial_read_write<SerT>::value;
    }


    template<typename T> constexpr
    type_spec spec_of()
    {
        return std::is_arithmetic<T>::value              ? type_spec::as_plain
                      : impl::has_serial_read_write<T>() ? type_spec::as_own_read_write
                      : impl::has_serial_tuple<T>()      ? type_spec::as_serial_tuple
                      : impl::is_container<T>()          ? type_spec::as_container
                      : impl::is_tuple<T>()              ? type_spec::as_tuple
                                                         : type_spec::error;
    }


} // namespace impl
} // namespace s11n

#endif // TYPE_SPEC_H
