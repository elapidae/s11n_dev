#ifndef S11N_IMPL_SIGNATURE_3_SERIAL_TUPLE_H
#define S11N_IMPL_SIGNATURE_3_SERIAL_TUPLE_H


#include "impl/signature_2_description.h"


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
    //      signature_3 = signature_2 + serial tuple specialization
    //===================================================================================
    enum class sign_spec_3
    {
        with_serial_tuple,
        without_serial_tuple
    };
    //-----------------------------------------------------------------------------------
    template <typename T>
    constexpr sign_spec_3 sign_spec_3_of()
    {
        return has_serial_tuple<T>() ? sign_spec_3::with_serial_tuple
                                     : sign_spec_3::without_serial_tuple;
    }
    //===================================================================================
    template<typename T,sign_spec_3>
    struct _signature_3;
    //-----------------------------------------------------------------------------------
    template<typename T>
    struct _signature_3<T,sign_spec_3::without_serial_tuple>
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
    //-----------------------------------------------------------------------------------
    template<typename T>
    struct _signature_3<T,sign_spec_3::with_serial_tuple>
    {
        //-------------------------------------------------------------------------------
        using Tup = decltype( s11n::Serial<T>::to_tuple(std::declval<T>()) );
        //-------------------------------------------------------------------------------
        static constexpr crc_type crc( crc_type prev )
        {
            return calc_crc_T<Tup>( prev );
        }
        //-------------------------------------------------------------------------------
        static void signature( std::string *res )
        {
            res->append( impl::signature<Tup>() );
        }
        //-------------------------------------------------------------------------------
    };
    //-----------------------------------------------------------------------------------
    template <typename T>
    std::string signature_3()
    {
        auto res = signature_2<T>();
        _signature_3<T,sign_spec_3_of<T>()>::signature( &res );
        return res;
    }
    //-----------------------------------------------------------------------------------
    template <typename T>
    constexpr crc_type calc_crc_3( crc_type prev )
    {
        return _signature_3<T,sign_spec_3_of<T>()>::crc
                (
                    calc_crc_2<T>( prev )
                );
    }
    //===================================================================================
    //      signature_3 = signature_2 + serial tuple specialization
    //===================================================================================
} // namespace impl
} // namespace s11n
//=======================================================================================

#endif // S11N_IMPL_SIGNATURE_3_SERIAL_TUPLE_H
