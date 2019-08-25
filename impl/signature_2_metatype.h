#ifndef SIGNATURE_2_METATYPE_H
#define SIGNATURE_2_METATYPE_H


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
    //  signature_2: signature_1 + metaargs
    //===================================================================================
    enum class sign_spec_2
    {
        without_metatype,
        as_container,
        as_any_metatype,
    };
    //-----------------------------------------------------------------------------------
    template<typename T> constexpr
    bool _is_metatype( const T* )
    {
        return false;
    }
    //-----------------------------------------------------------------------------------
    template< template<typename...> class MetaT, typename ... Args > constexpr
    bool _is_metatype( const MetaT<Args...>* )
    {
        return true;
    }
    //-----------------------------------------------------------------------------------
    template<typename T> constexpr
    bool is_metatype()
    {
        return _is_metatype( static_cast<T*>(nullptr) );
    }
    //-----------------------------------------------------------------------------------
    template <typename T> constexpr
    sign_spec_2 sign_spec_2_of()
    {
        return is_container<T>()        ? sign_spec_2::as_container
            : impl::is_metatype<T>()    ? sign_spec_2::as_any_metatype
                                        : sign_spec_2::without_metatype;
    }
    //===================================================================================
    template <typename T, impl::sign_spec_2>
    struct _signature_2;
    //===================================================================================
    //  as_container
    template <typename T>
    struct _signature_2<T, impl::sign_spec_2::as_container>
    {
        //-------------------------------------------------------------------------------
        using _vt = typename T::value_type;
        //-------------------------------------------------------------------------------
        static void signature( std::string * res )
        {
            res->push_back( '<' );
            res->append( impl::signature<_vt>() );
            res->push_back( '>' );
        }
        //-------------------------------------------------------------------------------
        static constexpr crc_type crc( crc_type prev )
        {
            return calc_crc_ch( '>',
                        calc_crc_T<_vt>(
                            calc_crc_ch( '<', prev )
                        )
                    );
        }
        //-------------------------------------------------------------------------------
    };
    //  as_container
    //===================================================================================
    //  as_any_metatype
    //-----------------------------------------------------------------------------------
    //  signature of args
    template< typename ... Args >
    struct _signature_metaargs;
    //-----------------------------------------------------------------------------------
    template< typename T1, typename ... Args >
    struct _signature_metaargs<T1,Args...>
    {
        //-------------------------------------------------------------------------------
        constexpr static char delimiter() { return ','; }
        //-------------------------------------------------------------------------------
        static std::string signature(bool = false)
        {
            return impl::signature<T1>() +
                   _signature_metaargs<Args...>::delimiter() +
                   _signature_metaargs<Args...>::signature(true);
        }
        //-------------------------------------------------------------------------------
        static constexpr uint32_t crc( uint32_t prev, bool = false)
        {
            return
                _signature_metaargs<Args...>::crc
                (
                    calc_crc_ch
                    (
                        _signature_metaargs<Args...>::delimiter(),
                        calc_crc_T<T1>( prev )
                    ),
                    true
                );
        }
        //-------------------------------------------------------------------------------
    };
    //-----------------------------------------------------------------------------------
    template<>
    struct _signature_metaargs<>
    {
        //-------------------------------------------------------------------------------
        constexpr static char delimiter() { return '>'; }
        //-------------------------------------------------------------------------------
        static std::string signature(bool delim = false)
        {
            return delim ? "" : ">";
        }
        //-------------------------------------------------------------------------------
        static constexpr uint32_t crc( uint32_t prev, bool delim = false)
        {
            return delim ? prev : calc_crc_ch( '>', prev );
        }
        //-------------------------------------------------------------------------------
    };
    //-----------------------------------------------------------------------------------
    template< template<typename...> class MetaT, typename ... Args >
    std::string _signature_args( const MetaT<Args...>* )
    {
        return std::string("<") + _signature_metaargs<Args...>::signature();
    }
    //-----------------------------------------------------------------------------------
    template< template<typename...> class MetaT, typename ... Args >
    constexpr uint32_t _calc_crc_args( const MetaT<Args...>*, uint32_t prev )
    {
        return _signature_metaargs<Args...>::crc( calc_crc_ch('<', prev) );
    }
    //  signature of args
    //-----------------------------------------------------------------------------------
    //  _signature_2
    template <typename T>
    struct _signature_2<T, impl::sign_spec_2::as_any_metatype>
    {
        //-------------------------------------------------------------------------------
        static std::string signature()
        {
            return _signature_args( static_cast<T*>(nullptr) );
        }
        //-------------------------------------------------------------------------------
        static constexpr crc_type crc( crc_type prev )
        {
            return
                _calc_crc_args( static_cast<T*>(nullptr), prev );
        }
        //-------------------------------------------------------------------------------
    };
    //  as_any_metatype
    //===================================================================================
    //  without_metatype
    template <typename T>
    struct _signature_2<T, impl::sign_spec_2::without_metatype>
    {
        //-------------------------------------------------------------------------------
        static std::string signature()
        {
            return {};
        }
        //-------------------------------------------------------------------------------
        static constexpr crc_type crc( crc_type prev )
        {
            return prev;
        }
        //-------------------------------------------------------------------------------
    };
    //  without_metatype
    //===================================================================================
    template <typename T>
    std::string signature_2()
    {
        auto res = signature_1<T>();
        _signature_2<T,sign_spec_2_of<T>()>::signature();
    }
    //-----------------------------------------------------------------------------------
    template <typename T>
    constexpr crc_type calc_crc_2( crc_type prev )
    {
        return _signature_2<T,sign_spec_2_of<T>()>::crc( prev );
    }
    //===================================================================================
    //      signature_2
    //===================================================================================
}} // namespace s11n::impl
//=======================================================================================

#endif // SIGNATURE_2_METATYPE_H
