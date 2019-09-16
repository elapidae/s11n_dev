#ifndef S11N_IMPL_SIGNATURE_1_NAME_OF_TYPE_H
#define S11N_IMPL_SIGNATURE_1_NAME_OF_TYPE_H

#include "impl/crc.h"
#include "impl/name_of_type_from_pf.h"
#include "impl/tuple_helper.h"
#include "impl/container_helper.h"
#include "impl/metaargs_helper.h"


//=======================================================================================
//  signature_1:
//      if type is arithmetic: using plain name such as uint32
//      if type has Serial<T>::name_of_type specification: using this spec
//      if type is tuple: using {T1,T2,...,TN} spec
//      elsewhere using name of type from __PRETTY_FUNCTION__
//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    template <typename T>
    std::string signature_1();
    //===================================================================================
    template <typename T>
    constexpr crc_type calc_crc_1( crc_type prev );
    //===================================================================================
}}
//=======================================================================================



//=======================================================================================
//      Implementation
//=======================================================================================
namespace s11n { template <typename T> struct Serial; }
//===================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    //  Forward declarations
    //===================================================================================
    template <typename T>
    std::string signature();
    //-----------------------------------------------------------------------------------
    template <typename T> constexpr
    crc_type crc();
    //-----------------------------------------------------------------------------------
    template<typename T> constexpr
    crc_type calc_crc_T( crc_type prev );
    //===================================================================================


    //===================================================================================
    //  signature_1: extracting name of type
    //===================================================================================
    //      Serial<T>::name_of_type deduction
    template<typename T, typename = void>
    struct _has_serial_name_of_type : std::false_type {};
    //-----------------------------------------------------------------------------------
    template<typename T>
    struct _has_serial_name_of_type
            <
                T,
                typename std::void_t< decltype(s11n::Serial<T>::name_of_type) >
            >
        : std::true_type
    {};
    //-----------------------------------------------------------------------------------
    template<typename T> constexpr
    bool has_serial_name_of_type()
    {
        static_assert ( !_has_serial_name_of_type<T>::value ||
                        !std::is_arithmetic<T>::value,
                        "Arithmetic types cannot have name_of_type." );

        return _has_serial_name_of_type<T>::value;
    }
    //      Serial<T>::name_of_type deduction
    //===================================================================================
    //  Первая часть расчета сигнатуры -- имя класса или Serial<T>::name_of_type.
    //  В свою очередь, имя класса для контейнеров и кортежей расчитывается рекурсивно.
    //  UPD 2019-08-22 -- добавляем вывод сигнатуры произвольных метатипов, но только
    //  после контейнеров.
    enum class sign_spec_1
    {
        as_plain,
        as_own_name_of_type,
        as_tuple,
        as_pretty_func
    };
    //-----------------------------------------------------------------------------------
    template <typename T> constexpr
    sign_spec_1 sign_spec_1_of()
    {
        return    std::is_arithmetic<T>::value        ? sign_spec_1::as_plain
                : impl::has_serial_name_of_type<T>()  ? sign_spec_1::as_own_name_of_type
                : impl::is_tuple<T>()                 ? sign_spec_1::as_tuple
                                                      : sign_spec_1::as_pretty_func;
    }
    //===================================================================================
    template <typename T, impl::sign_spec_1>
    struct _signature_1;
    //===================================================================================
    //  as plain
    template <typename T>
    struct _signature_1<T, impl::sign_spec_1::as_plain>
    {
        //-------------------------------------------------------------------------------
        static constexpr str_view name_of_type()
        {
            static_assert( sizeof(bool)   == 1, "sizeof(bool)   != 1" );
            static_assert( sizeof(char)   == 1, "sizeof(char)   != 1" );
            static_assert( sizeof(float)  == 4, "sizeof(float)  != 4" );
            static_assert( sizeof(double) == 8, "sizeof(double) != 8" );
            static_assert( sizeof(T)      <= 8, "size is too big..."  );

            using TT = typename std::remove_cv<T>::type;

            return
                std::is_same<char,TT>::value     ? impl::str_view{"char", 4}    :
                std::is_same<bool,TT>::value     ? impl::str_view{"bool", 4}    :

                std::is_same<int8_t,TT>::value   ? impl::str_view{"int8", 4}    :
                std::is_same<int16_t,TT>::value  ? impl::str_view{"int16", 5}   :
                std::is_same<int32_t,TT>::value  ? impl::str_view{"int32", 5}   :
                std::is_same<int64_t,TT>::value  ? impl::str_view{"int64", 5}   :

                std::is_same<uint8_t,TT>::value  ? impl::str_view{"uint8", 5}   :
                std::is_same<uint16_t,TT>::value ? impl::str_view{"uint16", 6}  :
                std::is_same<uint32_t,TT>::value ? impl::str_view{"uint32", 6}  :
                std::is_same<uint64_t,TT>::value ? impl::str_view{"uint64", 6}  :

                std::is_same<float,TT>::value    ? impl::str_view{"float32", 7} :
                std::is_same<double,TT>::value   ? impl::str_view{"float64", 7} :
                                                   impl::str_view{"", 0};
        }
        //-------------------------------------------------------------------------------
        static std::string sign()
        {
            static_assert( name_of_type().len > 0,
                           "Error during depending of arithmetic type" );

            return name_of_type();
        }
        //-------------------------------------------------------------------------------
        static constexpr crc_type crc( crc_type prev )
        {
            static_assert( name_of_type().len > 0,
                           "Error during depending of arithmetic type" );

            return calc_crc_str( name_of_type(), prev );
        }
        //-------------------------------------------------------------------------------
    };
    //  as_plain
    //===================================================================================
    //  as_own_name_of_type
    template <typename T>
    struct _signature_1<T, impl::sign_spec_1::as_own_name_of_type>
    {
        //-------------------------------------------------------------------------------
        static constexpr str_view name_of_type()
        {
            return
            {
                ::s11n::Serial<T>::name_of_type,
                str_length( ::s11n::Serial<T>::name_of_type )
            };
        }
        //-------------------------------------------------------------------------------
        static std::string sign()
        {
            return name_of_type();
        }
        //-------------------------------------------------------------------------------
        static constexpr crc_type crc( crc_type prev )
        {
            return calc_crc_str( name_of_type(), prev );
        }
        //-------------------------------------------------------------------------------
    };
    //  as_own_name_of_type
    //===================================================================================
    //  as_tuple
    template <typename T>
    struct _signature_1<T, impl::sign_spec_1::as_tuple>
    {
        //-------------------------------------------------------------------------------
        static std::string sign()
        {
            return std::string("{") +
                   signature_metaargs(static_cast<T*>(nullptr)) +
                   std::string("}");
        }
        //-------------------------------------------------------------------------------
        static constexpr crc_type crc( crc_type prev )
        {
            return  calc_crc_ch( '}',
                        calc_crc_metaargs(static_cast<T*>(nullptr),
                            calc_crc_ch( '{', prev )
                        )
                    );
        }
        //-------------------------------------------------------------------------------
    };
    //  as_tuple
    //===================================================================================
    //  as_pretty_func
    template <typename T>
    struct _signature_1<T, impl::sign_spec_1::as_pretty_func>
    {
        //-------------------------------------------------------------------------------
        static constexpr str_view name_of_type()
        {
            return name_of_type_from_PF<T>();
        }
        //-------------------------------------------------------------------------------
        static std::string sign()
        {
            return name_of_type();
        }
        //-------------------------------------------------------------------------------
        static constexpr crc_type crc( crc_type prev )
        {
            return calc_crc_str( name_of_type(), prev );
        }
        //-------------------------------------------------------------------------------
    };
    //  as_pretty_func
    //===================================================================================
    template <typename T>
    std::string signature_1()
    {
        return _signature_1<T,sign_spec_1_of<T>()>::sign();
    }
    //-----------------------------------------------------------------------------------
    template <typename T>
    constexpr crc_type calc_crc_1( crc_type prev )
    {
        return _signature_1<T,sign_spec_1_of<T>()>::crc( prev );
    }
    //===================================================================================
    //      signature_1
    //===================================================================================
} // namespace impl
} // namespace s11n
//=======================================================================================


#endif // S11N_IMPL_SIGNATURE_1_NAME_OF_TYPE_H
