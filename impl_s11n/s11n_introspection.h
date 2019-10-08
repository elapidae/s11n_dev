#ifndef S11N_INTROSPECTION_H
#define S11N_INTROSPECTION_H

#include <stddef.h>
#include <string>

//=======================================================================================
//      Face: use extract_typename<T>()
//=======================================================================================
namespace s11n
{
    //===================================================================================
    template <typename T> constexpr const char* _pos_T_in_pattern();
    constexpr size_t _index_of( const char *src, char pattern );
    //===================================================================================
    struct _strview
    {
        const char *ptr;
        size_t len;

        std::string str() const         { return {ptr, len}; }
    };
    //===================================================================================

    //===================================================================================
    template <typename T> constexpr const
    _strview  name_of_type();
    //===================================================================================
    template <typename T> constexpr const std::enable_if<std::is_arithmetic<T>::value,
    _strview> name_of_type();
    //===================================================================================
    template <typename T>
    constexpr const std::enable_if<std::is_base_of<std::string,T>::value,
    _strview> name_of_type();
//    template <typename T>
//    constexpr const _strview _name_of_type_from_PF();
    //===================================================================================
}
//=======================================================================================
//      Face: use extract_typename<T>()
//=======================================================================================


//=======================================================================================
//      Implementation
//=======================================================================================
namespace s11n
{
    //===================================================================================
    //      name_of_type()
    //===================================================================================
    template <typename T>
    constexpr const std::enable_if<std::is_arithmetic<T>::value, _strview>
    name_of_type()
    {
        static_assert( sizeof(char)   == 1, "sizeof(char)   != 1" );
        static_assert( sizeof(float)  == 4, "sizeof(float)  != 4" );
        static_assert( sizeof(double) == 8, "sizeof(double) != 8" );

        return
            std::is_same<char,T>::value     ? _strview{"char", 4}    :
            std::is_same<bool,T>::value     ? _strview{"bool", 4}    :

            std::is_same<int8_t,T>::value   ? _strview{"int8", 4}    :
            std::is_same<int16_t,T>::value  ? _strview{"int16", 5}   :
            std::is_same<int32_t,T>::value  ? _strview{"int32", 5}   :

            std::is_same<uint8_t,T>::value  ? _strview{"uint8", 5}   :
            std::is_same<uint16_t,T>::value ? _strview{"uint16", 6}  :
            std::is_same<uint32_t,T>::value ? _strview{"uint32", 6}  :

            std::is_same<int64_t,T>::value  ? _strview{"int64", 5}   :
            std::is_same<uint64_t,T>::value ? _strview{"uint64", 6}  :

            std::is_same<float,T>::value    ? _strview{"float32", 7} :
            std::is_same<double,T>::value   ? _strview{"float64", 7} :
                                              _strview{"!!! not arithmetic !!!", 22};
    }
    //===================================================================================
    template <typename T>
    constexpr const std::enable_if<std::is_base_of<std::string,T>::value,
    _strview> name_of_type()
    {
        return {"string",6};
    }
    //===================================================================================
    //      name_of_type()
    //===================================================================================


    //===================================================================================
    //  Принцип извлечения имени типа:
    //   - вызываем функцию _sign_T_PRETTY_FUNC<Type>(), которая возвращает
    //      __PRETTY_FUNCTION__, в ней, так или иначе, хранится имя типа.
    //      Дальнейшая задача -- выковырять это имя в compile-time.
    //
    template <typename T>
    constexpr const _strview name_of_type_from_PF()
    {
        return
        {
            _pos_T_in_pattern<T>(),
            _index_of( _pos_T_in_pattern<T>(), ']' )
        };
    }
    //===================================================================================
    //  "Сырая" подпись типа через __PRETTY_FUNCTION__.
    //  Имя типа хранится в этой строке, его надо извлечь: выводим в консоль результат
    //  функции с любым типом, копируем текст до имени типа
    //  в переменную _raw_sign_T_preambul.
    //  Дальнейшее извлечение будет вычитать эту строку из начала.
    //
    template <typename T>
    constexpr const char * _sign_T_PRETTY_FUNC()
    {
        return __PRETTY_FUNCTION__;
    }
    //  Если менять имена функций и namepsace-ов, то надо руками откорректировать
    //  эту константу:
    //      1. cout << s11n::_sign_T_PRETTY_FUNC<int>() << endl;
    //      2. copy result until "int]";
    constexpr auto _sign_T_preambul =
    "constexpr const char* s11n::_sign_T_PRETTY_FUNC() [with T = ";
    //===================================================================================
    //  Возвращает указатель, следующий за паттерном.
    //  По идее, должен проверять правильность паттерна вначале. Пока не знаю как
    //  это реализовать в 11-м стандарте.
    constexpr const char* _advance_along_substr( const char *src, const char *pattern )
    {
        // cannot do: static_assert( *src == *pattern, "" );
        return *pattern == '\0'
                    ? src
                    : _advance_along_substr( src+1, pattern+1 );
    }
    //===================================================================================
    //  Ищем начало имени типа в __PRETTY_FUNC__
    template <typename T>
    constexpr const char* _pos_T_in_pattern()
    {
        return _advance_along_substr( _sign_T_PRETTY_FUNC<T>(), _sign_T_preambul );
    }
    //
    //===================================================================================
    //  Индекс первого найденного символа в строке, в рекурсии.
    //  См. _index_of()
    constexpr size_t _index_of_with_recurse( const char *src, char pattern, size_t accum)
    {
        //  cannot in c++11: static_assert ( *src != '\0', "" );
        return *src == pattern
                    ? accum
                    : _index_of_with_recurse( src + 1, pattern, accum + 1 );
    }
    //===================================================================================
    //  Индекс первого найденного символа в строке. Здесь вход в рекурсию.
    constexpr size_t _index_of( const char *src, char pattern )
    {
        return _index_of_with_recurse( src, pattern, 0 );
    }
    //===================================================================================
}
//=======================================================================================
//      Implementation
//=======================================================================================


#endif // S11N_INTROSPECTION_H
