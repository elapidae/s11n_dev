#ifndef SIGNATURE_H
#define SIGNATURE_H

#include "vlog.h"

#include "impl/tuple_helper.h"
#include "impl/name_of_type.h"
#include "impl/description.h"

namespace s11n {
namespace impl
{
    //===================================================================================
    //
    template <typename T> static typename
    std::enable_if< is_tuple<T>(), std::string>::type signature();

    template <typename T> static typename
    std::enable_if< has_serial_tuple<T>(), std::string>::type signature();

    template <typename T> static typename
    std::enable_if< !is_tuple<T>() &&
                    !has_serial_tuple<T>(), std::string>::type signature();
    //===================================================================================

    template <typename T>
    using serial_tuple_type = decltype
    (
        s11n::Serial<T>::to_tuple( *static_cast<T*>(nullptr) )
    );


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
    template <typename T> static typename
    std::enable_if
    <
        !is_tuple<T>() && !has_serial_tuple<T>(),
        std::string
    >::type
    signature()
    {
        return name_of_type<T>().str() +
               description_in_squares_str<T>();
    }
    //-----------------------------------------------------------------------------------
    template <typename T> static typename
    std::enable_if
    <
        is_tuple<T>(),
        std::string
    >::type
    signature()
    {
        return tuple_signature<T>();
    }
    //-----------------------------------------------------------------------------------
    template <typename T> static typename
    std::enable_if
    <
        has_serial_tuple<T>(),
        std::string
    >::type
    signature()
    {
        return name_of_type<T>().str() +
               description_in_squares_str<T>() +
               tuple_signature< serial_tuple_type<T> >();
    }
    //===================================================================================
} // namespace impl
} // namespace s11n
//=======================================================================================

#endif // SIGNATURE_H
