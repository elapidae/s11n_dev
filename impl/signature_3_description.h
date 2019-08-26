#ifndef S11N_IMPL_SIGNATURE_3_DESCRIPTION_H
#define S11N_IMPL_SIGNATURE_3_DESCRIPTION_H

#include "impl/signature_2_metatype.h"


//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    template <typename T>
    std::string signature_3();
    //===================================================================================
    template <typename T>
    constexpr crc_type calc_crc_3( crc_type prev );
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
    //      signature_3 = signature_2 + description
    //===================================================================================
    //      Serial<T>::description deduction
    template<typename T, typename = void>
    struct _has_serial_description : std::false_type {};
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
    template<typename T> constexpr
    bool has_serial_description()
    {
        static_assert ( !_has_serial_description<T>::value ||
                        !std::is_arithmetic<T>::value,
                        "Arithmetic types cannot have description." );

        return _has_serial_description<T>::value;
    }
    //      Serial<T>::description deduction
    //===================================================================================
    enum class sign_spec_3
    {
        with_description,
        without_description
    };
    //-----------------------------------------------------------------------------------
    template<typename T>
    constexpr sign_spec_3 sign_spec_3_of()
    {
        return  has_serial_description<T>() ? sign_spec_3::with_description
                                            : sign_spec_3::without_description;
    }
    //===================================================================================
    template<typename T, sign_spec_3>
    struct _signature_3;
    //===================================================================================
    template<typename T>
    struct _signature_3<T,sign_spec_3::without_description>
    {
        //-------------------------------------------------------------------------------
        static constexpr crc_type crc( crc_type prev )
        {
            return prev;
        }
        //-------------------------------------------------------------------------------
        static std::string sign()
        {
            return {};
        }
        //-------------------------------------------------------------------------------
    };
    //===================================================================================
    template<typename T>
    struct _signature_3<T,sign_spec_3::with_description>
    {
        //-------------------------------------------------------------------------------
        static constexpr impl::str_view description()
        {
            return
            {
                s11n::Serial<T>::description,
                str_length(s11n::Serial<T>::description)
            };
        }
        //-------------------------------------------------------------------------------
        static constexpr crc_type crc( crc_type prev )
        {
            return calc_crc_ch
                    (
                        ')',
                        calc_crc_str
                        (
                            description(),
                            calc_crc_ch( '(', prev )
                        )
                    );
        }
        //-------------------------------------------------------------------------------
        static std::string sign()
        {
            return std::string("(") + description().str() + ')';
        }
        //-------------------------------------------------------------------------------
    };
    //-----------------------------------------------------------------------------------
    template <typename T>
    std::string signature_3()
    {
        return signature_2<T>() +
              _signature_3<T,sign_spec_3_of<T>()>::sign();
    }
    //-----------------------------------------------------------------------------------
    template <typename T>
    constexpr crc_type calc_crc_3( crc_type prev )
    {
        return
            _signature_3<T,sign_spec_3_of<T>()>::crc
            (
                calc_crc_2<T>( prev )
            );
    }
    //===================================================================================
    //      signature_3 = signature_2 + description
    //===================================================================================
}} // namespace s11n::impl
//=======================================================================================


#endif // S11N_IMPL_SIGNATURE_3_DESCRIPTION_H
