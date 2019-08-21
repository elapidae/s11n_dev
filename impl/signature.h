#ifndef S11N_IMPL_SIGNATURE_H
#define S11N_IMPL_SIGNATURE_H

#include "impl/crc.h"
#include "impl/name_of_type_from_pf.h"
#include "impl/tuple_helper.h"
#include "impl/container_helper.h"

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
        return _has_serial_name_of_type<T>::value;
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
    sign_spec_1 sign_spec_1_of()
    {
        return  std::is_arithmetic<T>::value        ? sign_spec_1::as_plain
              : impl::has_serial_name_of_type<T>()  ? sign_spec_1::as_own_name_of_type
              : impl::is_container<T>()             ? sign_spec_1::as_container
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
        static constexpr uint32_t crc( uint32_t prev )
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
        static constexpr uint32_t crc( uint32_t prev )
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
        static constexpr uint32_t crc( uint32_t prev )
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
        static constexpr uint32_t crc( uint32_t prev )
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
    constexpr uint32_t calc_crc_1( uint32_t prev )
    {
        return _signature_1<T,sign_spec_1_of<T>()>::crc( prev );
    }
    //===================================================================================
    //      signature_1
    //===================================================================================


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
        return has_serial_description<T>() ? sign_spec_2::with_description
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
    constexpr uint32_t calc_crc_2( uint32_t prev )
    {
        return _signature_2<T,sign_spec_2_of<T>()>::crc
                (
                    calc_crc_1<T>( prev )
                );
    }
    //===================================================================================
    //      signature_2 = signature_1 + description
    //===================================================================================


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
    struct _signature_3<T,sign_spec_3::with_serial_tuple>
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
    std::string signature_3()
    {
        auto res = signature_2<T>();
        _signature_3<T,sign_spec_3_of<T>()>::signature( &res );
        return res;
    }
    //-----------------------------------------------------------------------------------
    template <typename T>
    constexpr uint32_t calc_crc_3( uint32_t prev )
    {
        return _signature_3<T,sign_spec_3_of<T>()>::crc
                (
                    calc_crc_2<T>( prev )
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
        return signature_3<T>();
    }
    //-----------------------------------------------------------------------------------
    template <typename T>
    constexpr uint32_t calc_crc_T( uint32_t prev )
    {
        return calc_crc_3<T>( prev );
    }
    //-----------------------------------------------------------------------------------
    template <typename T>
    constexpr uint32_t crc()
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
