#ifndef S11N_SERIAL_H
#define S11N_SERIAL_H

#include <string>


namespace s11n
{

template <typename T>
struct Serial;


struct ct_string_view
{
    const char *ptr;
    size_t len;

    std::string str() const;
};
//===================================================================================

template <typename T>
struct Serial
{
    static constexpr ct_string_view name_of_type()
    {
        static_assert( sizeof(char)   == 1, "sizeof(char)   != 1" );
        static_assert( sizeof(float)  == 4, "sizeof(float)  != 4" );
        static_assert( sizeof(double) == 8, "sizeof(double) != 8" );

        return
            std::is_same<char,T>::value     ? ct_string_view{"char", 4}    :
            std::is_same<bool,T>::value     ? ct_string_view{"bool", 4}    :

            std::is_same<int8_t,T>::value   ? ct_string_view{"int8", 4}    :
            std::is_same<int16_t,T>::value  ? ct_string_view{"int16", 5}   :
            std::is_same<int32_t,T>::value  ? ct_string_view{"int32", 5}   :

            std::is_same<uint8_t,T>::value  ? ct_string_view{"uint8", 5}   :
            std::is_same<uint16_t,T>::value ? ct_string_view{"uint16", 6}  :
            std::is_same<uint32_t,T>::value ? ct_string_view{"uint32", 6}  :

            std::is_same<int64_t,T>::value  ? ct_string_view{"int64", 5}   :
            std::is_same<uint64_t,T>::value ? ct_string_view{"uint64", 6}  :

            std::is_same<float,T>::value    ? ct_string_view{"float32", 7} :
            std::is_same<double,T>::value   ? ct_string_view{"float64", 7} :
                                              ct_string_view{"!!! not arithmetic !!!", 22};
    }
};

} // namespace s11n

#endif // S11N_SERIAL_H
