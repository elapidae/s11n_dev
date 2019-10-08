#ifndef S11N_IMPL_METAARGS_HELPER_H
#define S11N_IMPL_METAARGS_HELPER_H

#include "impl_s11n/crc.h"

//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    template< template<typename...> class MetaT, typename ... Args >
    std::string signature_metaargs( const MetaT<Args...>* );
    //-----------------------------------------------------------------------------------
    template< template<typename...> class MetaT, typename ... Args >
    constexpr crc_type calc_crc_metaargs( const MetaT<Args...>*, crc_type prev );
    //===================================================================================
}} // namespace s11n::impl
//=======================================================================================



//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    template<typename T>
    std::string signature();
    //===================================================================================
    template<typename T> constexpr
    crc_type calc_crc_T( crc_type prev );
    //===================================================================================


    //===================================================================================
    //  signature of args
    template< typename ... Args >
    struct _signature_metaargs;
    //-----------------------------------------------------------------------------------
    template< typename T1, typename ... Args >
    struct _signature_metaargs<T1,Args...>
    {
        //-------------------------------------------------------------------------------
        static std::string sign( bool first = true )
        {
            return std::string(first ? "" : ",") +
                   impl::signature<T1>() +
                   _signature_metaargs<Args...>::sign( false );
        }
        //-------------------------------------------------------------------------------
        static constexpr crc_type crc( crc_type prev, bool first = true )
        {
            return
                _signature_metaargs<Args...>::crc
                (
                    calc_crc_T<T1>
                    (
                        first ? prev : calc_crc_ch( ',', prev )
                    ),
                    false // not first
                );
        }
        //-------------------------------------------------------------------------------
    };
    //-----------------------------------------------------------------------------------
    template<>
    struct _signature_metaargs<>
    {
        //-------------------------------------------------------------------------------
        static std::string sign( bool first = true )
        {
            (void) first;
            return {};
        }
        //-------------------------------------------------------------------------------
        static constexpr crc_type crc( crc_type prev, bool /*first*/ = true )
        {
            return prev;
        }
        //-------------------------------------------------------------------------------
    };
    //===================================================================================
    template< template<typename...> class MetaT, typename ... Args >
    std::string signature_metaargs( const MetaT<Args...>* )
    {
        return _signature_metaargs<Args...>::sign();
    }
    //-----------------------------------------------------------------------------------
    template< template<typename...> class MetaT, typename ... Args >
    constexpr crc_type calc_crc_metaargs( const MetaT<Args...>*, crc_type prev )
    {
        return _signature_metaargs<Args...>::crc( prev );
    }
    //  signature of args
    //===================================================================================
}} // namespace s11n::impl
//=======================================================================================

#endif // S11N_IMPL_METAARGS_HELPER_H
