#pragma once


#include "impl/tuple_helper.h"
#include "impl/name_of_type.h"
#include "impl/description.h"


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
}}
//=======================================================================================



//=======================================================================================
//      Implementation
//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    //
    template <typename T> typename
    std::enable_if< is_tuple<T>(), std::string>::type _signature();

    template <typename T> typename
    std::enable_if< has_serial_tuple<T>(), std::string>::type _signature();

    template <typename T> typename
    std::enable_if< !is_tuple<T>() &&
                    !has_serial_tuple<T>(), std::string>::type _signature();
    //===================================================================================


    //===================================================================================
    //  Итерируем до тех пор, пока не достигнем последнего элемента, когда достигнем,
    //  установим индекс в -1 и выйдем через специализированную структуру.
    template<int idx, typename atuple>
    struct _tuple_signature
    {
        static void make_signature( std::string* res )
        {
            using element = tuple_element<atuple,idx>;
            *res += signature<element>();
            res->push_back(',');
            constexpr auto next_idx = tuple_next_idx<idx,atuple>();
            _tuple_signature<next_idx,atuple>::make_signature( res );
        }
    };
    //-----------------------------------------------------------------------------------
    template<typename atuple>
    struct _tuple_signature<-1,atuple>
    {
        static void make_signature( std::string* )
        {} // do nothing.
    };
    //-----------------------------------------------------------------------------------
    template<typename atuple>
    std::string tuple_signature()
    {
        std::string res = "{";
        static constexpr auto idx = tuple_start_idx<atuple>();
        _tuple_signature<idx,atuple>::make_signature( &res );
        if (idx == -1)
            res.push_back('}');
        else
            res.back() = '}'; // replace last ',';
        return res;
    }
    //-----------------------------------------------------------------------------------
    template <typename T> typename
    std::enable_if
    <
        !is_tuple<T>() && !has_serial_tuple<T>(),
        std::string
    >::type
    _signature()
    {
        return name_of_type<T>().str() +
               description_in_squares_str<T>();
    }
    //-----------------------------------------------------------------------------------
    template <typename T> typename
    std::enable_if
    <
        is_tuple<T>(),
        std::string
    >::type
    _signature()
    {
        return tuple_signature<T>();
    }
    //-----------------------------------------------------------------------------------
    template <typename T> typename
    std::enable_if
    <
        has_serial_tuple<T>(),
        std::string
    >::type
    _signature()
    {
        return name_of_type<T>().str() +
               description_in_squares_str<T>() +
               tuple_signature< serial_tuple_type<T> >();
    }
    //===================================================================================


    //===================================================================================
    template <typename T>
    std::string signature()
    {
        return _signature<T>();
    }
    //===================================================================================
} // namespace impl
} // namespace s11n
//=======================================================================================
