#ifndef S11N_SERIAL_H
#define S11N_SERIAL_H

#include <string>
#include "../s11n/impl/str_view.h"
//#include "../s11n/impl/name_of_type_from_pf.h"

namespace s11n
{

template <typename T>
struct Serial;

//===================================================================================

////template <typename T>
//template <>
//struct Serial< typename std::enable_if<std::is_arithmetic<T>::value,T>::type >
//{
//    // static const std::string signature() -- only if can
//};

    template <typename T>
    constexpr const typename std::enable_if< std::is_arithmetic<T>::value,
    impl::str_view>::type name_of_type()
    {
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


} // namespace s11n

#endif // S11N_SERIAL_H
