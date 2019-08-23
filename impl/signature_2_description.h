#ifndef S11N_IMPL_SIGNATURE_2_DESCRIPTION_H
#define S11N_IMPL_SIGNATURE_2_DESCRIPTION_H

#include "impl/signature_1_name_of_type.h"


//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    template <typename T>
    std::string signature_2();
    //===================================================================================
    template <typename T>
    constexpr crc_type calc_crc_2( crc_type prev );
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
    //      signature_2 = signature_1 + description
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
    enum class sign_spec_2
    {
        with_description,
        without_description
    };
    //-----------------------------------------------------------------------------------
    template<typename T>
    constexpr sign_spec_2 sign_spec_2_of()
    {
        return  has_serial_description<T>() ? sign_spec_2::with_description
                                            : sign_spec_2::without_description;
    }
    //===================================================================================
    template<typename T, sign_spec_2>
    struct _signature_2;
    //===================================================================================
    template<typename T>
    struct _signature_2<T,sign_spec_2::without_description>
    {
        //-------------------------------------------------------------------------------
        static constexpr crc_type crc( crc_type prev )
        {
            return prev;
        }
        //-------------------------------------------------------------------------------
        static void signature( std::string * )
        {}
        //-------------------------------------------------------------------------------
    };
    //===================================================================================
    template<typename T>
    struct _signature_2<T,sign_spec_2::with_description>
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
        static void signature( std::string * res )
        {
            res->push_back( '(' );
            res->append( description() );
            res->push_back( ')' );
        }
        //-------------------------------------------------------------------------------
    };
    //-----------------------------------------------------------------------------------
    template <typename T>
    std::string signature_2()
    {
        auto res = signature_1<T>();
        _signature_2<T,sign_spec_2_of<T>()>::signature( &res );
        return res;
    }
    //-----------------------------------------------------------------------------------
    template <typename T>
    constexpr crc_type calc_crc_2( crc_type prev )
    {
        return
            _signature_2<T,sign_spec_2_of<T>()>::crc
            (
                calc_crc_1<T>( prev )
            );
    }
    //===================================================================================
    //      signature_2 = signature_1 + description
    //===================================================================================
} // namespace impl
} // namespace s11n
//=======================================================================================


#endif // S11N_IMPL_SIGNATURE_2_DESCRIPTION_H