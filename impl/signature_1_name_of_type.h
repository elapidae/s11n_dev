#ifndef S11N_IMPL_SIGNATURE_1_NAME_OF_TYPE_H
#define S11N_IMPL_SIGNATURE_1_NAME_OF_TYPE_H

#include "impl/crc.h"
#include "impl/name_of_type_from_pf.h"
#include "impl/tuple_helper.h"
#include "impl/container_helper.h"


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
        as_container,
        as_tuple,
        as_metatype,
        as_pretty_func
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
    sign_spec_1 sign_spec_1_of()
    {
        return    std::is_arithmetic<T>::value        ? sign_spec_1::as_plain
                : impl::has_serial_name_of_type<T>()  ? sign_spec_1::as_own_name_of_type
                : impl::is_container<T>()             ? sign_spec_1::as_container
                : impl::is_tuple<T>()                 ? sign_spec_1::as_tuple
                : impl::is_metatype<T>()              ? sign_spec_1::as_metatype
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
        static std::string signature()
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
                Serial<T>::name_of_type,
                str_length( Serial<T>::name_of_type )
            };
        }
        //-------------------------------------------------------------------------------
        static std::string signature()
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
    //  as_container
    template <typename T>
    struct _signature_1<T, impl::sign_spec_1::as_container>
    {
        //-------------------------------------------------------------------------------
        using _vt = typename T::value_type;
        //-------------------------------------------------------------------------------
        static constexpr str_view name_of_type()
        {
            return name_of_type_from_PF<T,'<'>();
        }
        //-------------------------------------------------------------------------------
        static std::string signature()
        {
            return name_of_type().str() + '<' + impl::signature<_vt>() + '>';
        }
        //-------------------------------------------------------------------------------
        static constexpr crc_type crc( crc_type prev )
        {
            return calc_crc_ch( '>',
                        calc_crc_T<_vt>(
                            calc_crc_ch( '<',
                                calc_crc_str( name_of_type(), prev )
                            )
                        )
                    );
        }
        //-------------------------------------------------------------------------------
    };
    //  as_container
    //===================================================================================
    //  as_tuple
    //-----------------------------------------------------------------------------------
    //  signature_tuple
    template <int idx, typename Tup>
    struct _signature_tuple
    {
        //-------------------------------------------------------------------------------
        static void signature( std::string * res )
        {
            using t = tuple_element<Tup,idx>;
            res->append( impl::signature<t>() );

            constexpr auto next_idx = tuple_next_idx<idx,Tup>();
            if (next_idx > 0)
                res->push_back(',');

            _signature_tuple<next_idx,Tup>::signature( res );
        }
        //-------------------------------------------------------------------------------
        static constexpr crc_type crc( crc_type prev )
        {
            using t = tuple_element<Tup,idx>;
            using next = _signature_tuple<tuple_next_idx<idx,Tup>(),Tup>;

            return
                next::crc
                (
                    calc_crc_ch
                    (
                        tuple_next_idx<idx,Tup>() > 0 ? ',' : '}',
                        calc_crc_T<t>( prev )
                    )
                );
        }
        //-------------------------------------------------------------------------------
    };
    //-----------------------------------------------------------------------------------
    template <typename Tup>
    struct _signature_tuple<-1,Tup>
    {
        //-------------------------------------------------------------------------------
        static void signature( std::string * res )
        {
            res->push_back( '}' );
        }
        //-------------------------------------------------------------------------------
        static constexpr crc_type crc( crc_type prev )
        {
            return tuple_size<Tup>() != 0 ? prev
                                          : calc_crc_ch( '}', prev );
        }
        //-------------------------------------------------------------------------------
    };
    //  signature_tuple
    //===================================================================================
    //  signature_1
    template <typename T>
    struct _signature_1<T, impl::sign_spec_1::as_tuple>
    {
        //-------------------------------------------------------------------------------
        static std::string signature()
        {
            std::string res;
            res.push_back( '{' );

            constexpr auto idx = tuple_start_idx<T>();
            _signature_tuple<idx,T>::signature( &res );

            return res;
        }
        //-------------------------------------------------------------------------------
        static constexpr crc_type crc( crc_type prev )
        {
            return
                _signature_tuple<tuple_start_idx<T>(),T>::crc
                (
                    calc_crc_ch( '{', prev )
                );
        }
        //-------------------------------------------------------------------------------
    };
    //  as_tuple
    //===================================================================================
    //  as_metatype
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
        using namespace impl;
        return _signature_metaargs<Args...>::crc
        (
            calc_crc_ch( '<', prev )
        );
    }
    //  signature of args
    //-----------------------------------------------------------------------------------
    //  _signature_1
    template <typename T>
    struct _signature_1<T, impl::sign_spec_1::as_metatype>
    {
        //-------------------------------------------------------------------------------
        static constexpr str_view name_of_type()
        {
            return name_of_type_from_PF<T, '<'>();
        }
        //-------------------------------------------------------------------------------
        static std::string signature()
        {
            return name_of_type().str() + _signature_args( static_cast<T*>(nullptr) );
        }
        //-------------------------------------------------------------------------------
        static constexpr crc_type crc( crc_type prev )
        {
            return
                _calc_crc_args
                (
                    static_cast<T*>(nullptr),
                    calc_crc_str( name_of_type(), prev )
                );
        }
        //-------------------------------------------------------------------------------
    };
    //  as_metatype
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
        static std::string signature()
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
        return _signature_1<T,sign_spec_1_of<T>()>::signature();
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
