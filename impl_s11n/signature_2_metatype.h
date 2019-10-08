#ifndef S11N_IMPL_SIGNATURE_2_METAARGS_H
#define S11N_IMPL_SIGNATURE_2_METAARGS_H


#include "impl_s11n/signature_1_name_of_type.h"
#include "impl_s11n/metaargs_helper.h"

//=======================================================================================
namespace s11n {
namespace impl_s11n
{
    //===================================================================================
    template <typename T>
    std::string signature_2_metaargs();
    //===================================================================================
    template <typename T>
    constexpr crc_type calc_crc_2_metaargs( crc_type prev );
    //===================================================================================
}} // namespace s11n::impl_s11n
//=======================================================================================



//=======================================================================================
//      Implementation
//=======================================================================================
namespace s11n {
namespace impl_s11n
{
    //===================================================================================
    //  signature_2: signature_1 + metaargs
    //===================================================================================
    enum class sign_spec_2
    {
        without_metatype,
        as_tuple,
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
        return is_tuple<T>()    ? sign_spec_2::as_tuple
            : is_container<T>() ? sign_spec_2::as_container
            : is_metatype<T>()  ? sign_spec_2::as_any_metatype
                                : sign_spec_2::without_metatype;
    }
    //===================================================================================
    template <typename T, sign_spec_2>
    struct _signature_2;
    //===================================================================================
    //  as_tuple
    template <typename T>
    struct _signature_2<T, sign_spec_2::as_tuple>
    {
        //-------------------------------------------------------------------------------
        static std::string sign()
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
    //  as_tuple
    //===================================================================================
    //  as_container
    template <typename T>
    struct _signature_2<T, sign_spec_2::as_container>
    {
        //-------------------------------------------------------------------------------
        using _vt = typename T::value_type;
        //-------------------------------------------------------------------------------
        static std::string sign()
        {
            return std::string("<") +
                   signature<_vt>() +
                   std::string(">");
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
    //  _signature_2
    template <typename T>
    struct _signature_2<T, sign_spec_2::as_any_metatype>
    {
        //-------------------------------------------------------------------------------
        static std::string sign()
        {
            return std::string("<") +
                   signature_metaargs( static_cast<T*>(nullptr) ) +
                   std::string(">");
        }
        //-------------------------------------------------------------------------------
        static constexpr crc_type crc( crc_type prev )
        {
            return  calc_crc_ch( '>',
                        calc_crc_metaargs(static_cast<T*>(nullptr),
                            calc_crc_ch( '<', prev )
                        )
                    );
        }
        //-------------------------------------------------------------------------------
    };
    //  as_any_metatype
    //===================================================================================
    //  without_metatype
    template <typename T>
    struct _signature_2<T, sign_spec_2::without_metatype>
    {
        //-------------------------------------------------------------------------------
        static std::string sign()
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
    std::string signature_2_metaargs()
    {
        return signature_1<T>() +
              _signature_2<T,sign_spec_2_of<T>()>::sign();
    }
    //-----------------------------------------------------------------------------------
    template <typename T>
    constexpr crc_type calc_crc_2_metaargs( crc_type prev )
    {
        return _signature_2<T,sign_spec_2_of<T>()>::crc
               (
                    calc_crc_1<T>( prev )
               );
    }
    //===================================================================================
    //      signature_2
    //===================================================================================
}} // namespace s11n::impl_s11n
//=======================================================================================

#endif // S11N_IMPL_SIGNATURE_2_METAARGS_H
