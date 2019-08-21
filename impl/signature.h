#pragma once

#include "impl/crc.h"
#include "impl/name_of_type_from_pf.h"
#include "impl/tuple_helper.h"
#include "impl/type_spec.h"

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
//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    template <typename T>
    std::string signature();
    //===================================================================================
    template <typename T> constexpr
    uint32_t crc();
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
    template<typename T>
    constexpr uint32_t calc_crc_T( uint32_t prev );
    //===================================================================================

    //===================================================================================
    //  signature_1: extracting name of type
    //===================================================================================
    //      Serial<T>::name_of_type deduction
    template<typename T, typename = void>
    struct _s_has_serial_name_of_type : std::false_type {};
    //-----------------------------------------------------------------------------------
    template<typename T>
    struct _s_has_serial_name_of_type
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
        return _s_has_serial_name_of_type<T>::value;
    }
    //      Serial<T>::name_of_type deduction
    //===================================================================================
    //  Первая часть расчета сигнатуры -- имя класса или Serial<T>::name_of_type.
    //  В свою очередь, имя класса для контейнеров и кортежей расчитывается рекурсивно.
    enum class sign_spec_1
    {
        as_plain,
        as_own_name_of_type,
        as_container,
        as_tuple,
        as_pretty_func
    };
    //-----------------------------------------------------------------------------------
    template <typename T> constexpr
    sign_spec_1 _sign_spec_1_of()
    {
        return  std::is_arithmetic<T>::value        ? sign_spec_1::as_plain
              : impl::has_serial_name_of_type<T>()  ? sign_spec_1::as_own_name_of_type
              : impl::is_container<T>()             ? sign_spec_1::as_container
              : impl::is_tuple<T>()                 ? sign_spec_1::as_tuple
                                                    : sign_spec_1::as_pretty_func;
    }
    //===================================================================================
    template <typename T, impl::sign_spec_1>
    struct _s_signature_1;
    //===================================================================================
    //  as plain
    template <typename T>
    struct _s_signature_1<T, impl::sign_spec_1::as_plain>
    {
        //-------------------------------------------------------------------------------
        static constexpr str_view name_of_type()
        {
            static_assert( sizeof(bool)   == 1, "sizeof(bool)   != 1" );
            static_assert( sizeof(char)   == 1, "sizeof(char)   != 1" );
            static_assert( sizeof(float)  == 4, "sizeof(float)  != 4" );
            static_assert( sizeof(double) == 8, "sizeof(double) != 8" );
            static_assert( sizeof(T)      <= 8, "size is too big..."  );

            return
                std::is_same<char,T>::value     ? impl::str_view{"char", 4}    :
                std::is_same<bool,T>::value     ? impl::str_view{"bool", 4}    :

                std::is_same<int8_t,T>::value   ? impl::str_view{"int8", 4}    :
                std::is_same<int16_t,T>::value  ? impl::str_view{"int16", 5}   :
                std::is_same<int32_t,T>::value  ? impl::str_view{"int32", 5}   :
                std::is_same<int64_t,T>::value  ? impl::str_view{"int64", 5}   :

                std::is_same<uint8_t,T>::value  ? impl::str_view{"uint8", 5}   :
                std::is_same<uint16_t,T>::value ? impl::str_view{"uint16", 6}  :
                std::is_same<uint32_t,T>::value ? impl::str_view{"uint32", 6}  :
                std::is_same<uint64_t,T>::value ? impl::str_view{"uint64", 6}  :

                std::is_same<float,T>::value    ? impl::str_view{"float32", 7} :
                std::is_same<double,T>::value   ? impl::str_view{"float64", 7} :
                                                  impl::str_view{"!ERROR!", 7};
        }
        //-------------------------------------------------------------------------------
        static std::string signature()
        {
            return name_of_type();
        }
        //-------------------------------------------------------------------------------
        static constexpr uint32_t crc( uint32_t prev )
        {
            return calc_crc_str( name_of_type(), prev );
        }
        //-------------------------------------------------------------------------------
    };
    //  as_plain
    //===================================================================================
    //  as_own_name_of_type
    template <typename T>
    struct _s_signature_1<T, impl::sign_spec_1::as_own_name_of_type>
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
        static constexpr uint32_t crc( uint32_t prev )
        {
            return calc_crc_str( name_of_type(), prev );
        }
        //-------------------------------------------------------------------------------
    };
    //  as_own_name_of_type
    //===================================================================================
    //  as_container
    template <typename T>
    struct _s_signature_1<T, impl::sign_spec_1::as_container>
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
        static constexpr uint32_t crc( uint32_t prev )
        {
            return crc_ch( '>',
                        calc_crc_T<_vt>(
                            calc_crc_ch( '>',
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
    template <int idx, typename Tup>
    struct _s_signature_tuple
    {
        //-------------------------------------------------------------------------------
        static void signature( std::string * res )
        {
            using t = tuple_element<Tup,idx>;
            res->append( impl::signature<t>() );

            constexpr auto next_idx = tuple_next_idx<idx,Tup>();
            if (next_idx > 0)
                res->push_back(',');

            _s_signature_tuple<next_idx,Tup>::signature( res );
        }
        //-------------------------------------------------------------------------------
        static constexpr uint32_t crc( uint32_t prev )
        {
            using t = tuple_element<Tup,idx>;
            using next = _s_signature_tuple<tuple_next_idx<idx,Tup>(),Tup>;

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
    struct _s_signature_tuple<-1,Tup>
    {
        //-------------------------------------------------------------------------------
        static void signature( std::string * res )
        {
            res->push_back( '}' );
        }
        //-------------------------------------------------------------------------------
        static constexpr uint32_t crc( uint32_t prev )
        {
            return tuple_size<Tup>() != 0 ? prev
                                          : calc_crc_ch( '}', prev );
        }
        //-------------------------------------------------------------------------------
    };
    //-----------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------
    template <typename T>
    struct _s_signature_1<T, impl::sign_spec_1::as_tuple>
    {
        //-------------------------------------------------------------------------------
        static std::string signature()
        {
            std::string res;
            res.push_back( '{' );

            constexpr auto idx = tuple_start_idx<T>();
            _s_signature_tuple<idx,T>::signature( &res );

            return res;
        }
        //-------------------------------------------------------------------------------
        static constexpr uint32_t crc( uint32_t prev )
        {
            return
                _s_signature_tuple<tuple_start_idx<T>(),T>::crc
                (
                    calc_crc_ch( '{', prev )
                );
        }
        //-------------------------------------------------------------------------------
    };
    //  as_tuple
    //===================================================================================
    //  as_pretty_func
    template <typename T>
    struct _s_signature_1<T, impl::sign_spec_1::as_pretty_func>
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
        static constexpr uint32_t crc( uint32_t prev )
        {
            return calc_crc_str( name_of_type(), prev );
        }
        //-------------------------------------------------------------------------------
    };
    //  as_pretty_func
    //===================================================================================
    template <typename T>
    std::string _signature_1()
    {
        return _s_signature_1< T, _sign_spec_1_of<T>() >::signature();
    }
    //-----------------------------------------------------------------------------------
    template <typename T>
    constexpr uint32_t _calc_crc_1( uint32_t prev )
    {
        return _s_signature_1< T, _sign_spec_1_of<T>() >::crc( prev );
    }
    //===================================================================================
    //      signature_1
    //===================================================================================


    //===================================================================================
    //      signature_2 = signature_1 + description
    //===================================================================================
    //      Serial<T>::description deduction
    template<typename T, typename = void>
    struct _s_has_serial_description : std::false_type {};
    //-----------------------------------------------------------------------------------
    template<typename T>
    struct _s_has_serial_description
            <
                T,
                typename std::void_t< decltype(s11n::Serial<T>::description) >
            >
        : std::true_type
    {};
    //-----------------------------------------------------------------------------------
    template<typename T> constexpr
    bool __has_serial_description()
    {
        return _s_has_serial_description<T>::value;
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
    constexpr sign_spec_2 _sign_spec_2_of()
    {
        return __has_serial_description<T>() ? sign_spec_2::with_description
                                             : sign_spec_2::without_description;
    }
    //===================================================================================
    template<typename T, sign_spec_2>
    struct _s_signature_2;
    //===================================================================================
    template<typename T>
    struct _s_signature_2<T,sign_spec_2::without_description>
    {
        //-------------------------------------------------------------------------------
        static constexpr uint32_t crc( uint32_t prev )
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
    struct _s_signature_2<T,sign_spec_2::with_description>
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
        static constexpr uint32_t crc( uint32_t prev )
        {
            return calc_crc_ch
                    ( ')',
                        calc_crc_str
                        ( description(),
                            calc_crc_ch( '(', prev )
                        )
                    );
        }
        //-------------------------------------------------------------------------------
        static void signature( std::string *res )
        {
            res->push_back( '(' );
            res->append( description() );
            res->push_back( ')' );
        }
        //-------------------------------------------------------------------------------
    };
    //-----------------------------------------------------------------------------------
    template <typename T>
    std::string _signature_2()
    {
        auto res = _signature_1<T>();
        _s_signature_2<T, _sign_spec_2_of<T>()>::signature( &res );
        return res;
    }
    //-----------------------------------------------------------------------------------
    template <typename T>
    constexpr uint32_t _calc_crc_2( uint32_t prev )
    {
        return _s_signature_2< T, _sign_spec_2_of<T>() >::crc
                (
                    _calc_crc_1<T>( prev )
                );
    }
    //===================================================================================
    //      signature_2 = signature_1 + description
    //===================================================================================


    //===================================================================================
    //      signature_3 = signature_2 + serial tuple specialization
    //===================================================================================
    enum class _sign_spec_3
    {
        with_serial_tuple,
        without_serial_tuple
    };
    //-----------------------------------------------------------------------------------
    template <typename T>
    constexpr _sign_spec_3 _sign_spec_3_of()
    {
        return has_serial_tuple<T>() ? _sign_spec_3::with_serial_tuple
                                     : _sign_spec_3::without_serial_tuple;
    }
    //===================================================================================
    template<typename T,_sign_spec_3>
    struct _s_signature_3;
    //-----------------------------------------------------------------------------------
    template<typename T>
    struct _s_signature_3<T,_sign_spec_3::without_serial_tuple>
    {
        //-------------------------------------------------------------------------------
        static constexpr uint32_t crc( uint32_t prev )
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
    struct _s_signature_3<T,_sign_spec_3::with_serial_tuple>
    {
        //-------------------------------------------------------------------------------
        using Tup = decltype( s11n::Serial<T>::to_tuple(std::declval<T>()) );
        //-------------------------------------------------------------------------------
        static constexpr uint32_t crc( uint32_t prev )
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
    std::string _signature_3()
    {
        auto res = _signature_2<T>();
        _s_signature_3<T, _sign_spec_3_of<T>()>::signature( &res );
        return res;
    }
    //-----------------------------------------------------------------------------------
    template <typename T>
    constexpr uint32_t _calc_crc_3( uint32_t prev )
    {
        return _s_signature_3< T, _sign_spec_3_of<T>() >::crc
                (
                    _calc_crc_2<T>( prev )
                );
    }
    //===================================================================================
    //      signature_3 = signature_2 + serial tuple specialization
    //===================================================================================

    //===================================================================================
    //      Result wrap
    //===================================================================================
    template <typename T>
    std::string signature()
    {
        return _signature_3<T>();
    }
    //-----------------------------------------------------------------------------------
    template <typename T>
    constexpr uint32_t calc_crc_T( uint32_t prev )
    {
        return _calc_crc_3<T>( prev );
    }
    //-----------------------------------------------------------------------------------
    template <typename T>
    constexpr uint32_t crc()
    {
        return _crc_last_xor ^ calc_crc_T<T>( _crc_first_val );
    }
    //===================================================================================
    //      Result wrap
    //===================================================================================


} // namespace impl
} // namespace s11n
//=======================================================================================
