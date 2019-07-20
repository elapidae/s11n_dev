#ifndef NAME_OF_TYPE_FROM_PF_H
#define NAME_OF_TYPE_FROM_PF_H

#include <cstddef>
#include "../s11n/impl/str_view.h"

//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    template <typename T>
    static constexpr const str_view name_of_type_from_PF();
    //===================================================================================
} // namespace impl
} // namespace s11n
//=======================================================================================




//=======================================================================================
//      Implementation
//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    template <typename T> static constexpr const char* _pos_T_in_pattern();
    static constexpr size_t _index_of( const char *src, char pattern );

    //===================================================================================
    //  Принцип извлечения имени типа:
    //   - вызываем функцию _sign_T_PRETTY_FUNC<Type>(), которая возвращает
    //      __PRETTY_FUNCTION__, в ней, так или иначе, хранится имя типа.
    //      Дальнейшая задача -- выковырять это имя в compile-time.
    //
    template <typename T>
    static constexpr const str_view name_of_type_from_PF()
    {
        return
        {
            _pos_T_in_pattern<T>(),
            _index_of( _pos_T_in_pattern<T>(), ']' )
        };
    }
    //===================================================================================
    //  "Сырая" подпись типа через __PRETTY_FUNCTION__.
    //
    //  Имя типа хранится в этой строке, его надо извлечь: выводим в консоль результат
    //  функции с любым типом, копируем текст до имени типа
    //  в переменную _sign_T_preambul.
    //
    //  Дальнейшее извлечение будет вычитать эту строку из начала.
    //
    template <typename T>
    static constexpr const char * _sign_T_PRETTY_FUNC()
    {
        return __PRETTY_FUNCTION__;
    }
    //  Если менять имена функций и namepsace-ов, то надо руками откорректировать
    //  эту константу:
    //      1. cout << s11n::_sign_T_PRETTY_FUNC<int>() << endl;
    //      2. copy result until "int]";
    static constexpr auto _sign_T_preambul =
    "constexpr const char* s11n::impl::_sign_T_PRETTY_FUNC() [with T = ";
    //===================================================================================
    //  Возвращает указатель, следующий за паттерном.
    //  По идее, должен проверять правильность паттерна вначале. Пока не знаю как
    //  это реализовать в 11-м стандарте.
    static constexpr const char* _advance_along_substr( const char *src, const char *pattern )
    {
        // cannot do: static_assert( *src == *pattern, "" );
        return *pattern == '\0'
                    ? src
                    : _advance_along_substr( src+1, pattern+1 );
    }
    //===================================================================================
    //  Ищем начало имени типа в __PRETTY_FUNC__
    template <typename T>
    static constexpr const char* _pos_T_in_pattern()
    {
        return _advance_along_substr( _sign_T_PRETTY_FUNC<T>(), _sign_T_preambul );
    }
    //
    //===================================================================================
    //  Индекс первого найденного символа в строке, в рекурсии.
    //  См. _index_of()
    static constexpr size_t _index_of_with_recurse( const char *src, char pattern, size_t accum)
    {
        //  cannot in c++11: static_assert ( *src != '\0', "" );
        return *src == pattern
                    ? accum
                    : _index_of_with_recurse( src + 1, pattern, accum + 1 );
    }
    //===================================================================================
    //  Индекс первого найденного символа в строке. Здесь вход в рекурсию.
    static constexpr size_t _index_of( const char *src, char pattern )
    {
        return _index_of_with_recurse( src, pattern, 0 );
    }
    //===================================================================================
} // impl
} // s11n
//=======================================================================================
//      Implementation
//=======================================================================================



#endif // NAME_OF_TYPE_FROM_PF_H
