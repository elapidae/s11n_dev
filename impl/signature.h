#ifndef S11N_IMPL_SIGNATURE_H
#define S11N_IMPL_SIGNATURE_H

#include "impl/signature_4_serial_tuple.h"

//=======================================================================================
//      signature -- сигнатура типа, что-то вроде отпечатка пальца.
//
//  Сигнатурой считается имя типа + необязательное описание + его отображение на кортеж.
//
//  - имя типа -- см. name_of_type;
//  - описание -- см. description;
//  - отображение на кортеж -- см. tuple_helper;
//
//
//  Сигнатура арифметических типов совпадает с их name_of_type.
//
//  Сигнатура кортежей составляется из фигурных скобок, в которых указываются типы:
//      "{name_of_type1,name_of_type2,...,name_of_typeN}"
//
//
//  Под капотом: сделано в три стадии: <имя_типа> -> <описание> -> <кортеж>
//
//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    template <typename T>
    std::string signature();
    //===================================================================================
    template <typename T> constexpr
    crc_type signature_crc();
    //===================================================================================
}}
//=======================================================================================



//=======================================================================================
//      Implementation
//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    //      Result wrap
    //===================================================================================
    template <typename T> constexpr
    bool test_type()
    {
        static_assert ( !std::is_pointer<T>::value,
                        "s11n::signature don't work with pointers." );
        static_assert ( !std::is_member_pointer<T>::value,
                        "s11n::signature don't work with member pointers." );
        static_assert ( !std::is_member_function_pointer<T>::value,
                        "s11n::signature don't work with member function pointers." );
        static_assert ( !std::is_function<T>::value,
                        "s11n::signature don't work with functions." );
        static_assert ( !std::is_array<T>::value,
                        "s11n::signature don't work with arrays. "
                        "Use std::array instead." );
        return true;
    }
    //===================================================================================
    template <typename T>
    std::string signature()
    {
        static_assert( test_type<T>(), "");
        return signature_4<T>();
    }
    //-----------------------------------------------------------------------------------
    template <typename T> constexpr
    crc_type calc_crc_T( crc_type prev )
    {
        static_assert( test_type<T>(), "");
        return calc_crc_4<T>( prev );
    }
    //-----------------------------------------------------------------------------------
    template <typename T> constexpr
    crc_type signature_crc()
    {
        return crc_last_xor ^ calc_crc_T<T>( crc_first_val );
    }
    //===================================================================================
    //      Result wrap
    //===================================================================================
} // namespace impl
} // namespace s11n
//=======================================================================================

#endif // S11N_IMPL_SIGNATURE_H
