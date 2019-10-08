#ifndef S11N_IMPL_NAME_OF_TYPE_FROM_PF_H
#define S11N_IMPL_NAME_OF_TYPE_FROM_PF_H

#include "impl_s11n/str_view.h"

//=======================================================================================
//  UPD 2019-09-24
//  На старом компиляторе не работает constexpr __PRETTY_FUNC__
//  Приходится макросню разводить, чтобы выяснить что происходит, пусть уж лучше
//  в рантайме работает, чем ломается.
//  Определяем пару макросов, притом проверяем наличие GNUC. Столь странно,
//  чтобы работало на других компиляторах.
//  Про макросы для определения компиляторов:
/* https://blog.kowalczyk.info/article/j/
guide-to-predefined-macros-in-c-compilers-gcc-clang-msvc-etc..html */
#define S11N_CAN_CONSTEXPR_PRETTY_FUNC
#ifndef __clang__
    #ifdef __GNUC__
        #if ( ((__GNUC__<<16) | (__GNUC_MINOR__<<8) | __GNUC_PATCHLEVEL__) < 0x050000 )
            #undef S11N_CAN_CONSTEXPR_PRETTY_FUNC
        #endif // version <= 5.0
    #endif // __GNUC__
#endif // __clang__

#ifdef S11N_CAN_CONSTEXPR_PRETTY_FUNC
    #define S11N_CONSTEXPR_PRETTY_FUNC constexpr
#else
    #define S11N_CONSTEXPR_PRETTY_FUNC
#endif
//=======================================================================================


//=======================================================================================
namespace s11n {
namespace impl_s11n
{
    //===================================================================================
    //  terminal_ch добавлен, чтобы в дальнейшем подцеплять имена контейнеров до их
    //  аргументов (до '<').
    template <typename T>
    S11N_CONSTEXPR_PRETTY_FUNC
    str_view name_of_type_from_PF();
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
    template <typename T> static constexpr const char* _pos_T_in_pattern();

    //===================================================================================
    //  Принцип извлечения имени типа:
    //   - вызываем функцию _sign_T_PRETTY_FUNC<Type>(), которая возвращает
    //      __PRETTY_FUNCTION__, в ней, так или иначе, хранится имя типа.
    //      Дальнейшая задача -- выковырять это имя в compile-time.
    //
    template <typename T>
    S11N_CONSTEXPR_PRETTY_FUNC
    str_view name_of_type_from_PF()
    {
        return
        {
            _pos_T_in_pattern<T>(),

            str_index_of(_pos_T_in_pattern<T>(),']') <
            str_index_of(_pos_T_in_pattern<T>(),'<')
                    ? str_index_of(_pos_T_in_pattern<T>(),']')
                    : str_index_of(_pos_T_in_pattern<T>(),'<')
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
    S11N_CONSTEXPR_PRETTY_FUNC
    const char * _sign_T_PRETTY_FUNC()
    {
        return __PRETTY_FUNCTION__;
    }
    //  Если менять имена функций и namepsace-ов, то надо руками откорректировать
    //  эту константу:
    //      1. cout << s11n::impl_s11n::_sign_T_PRETTY_FUNC<int>() << endl;
    //      2. copy result until "int]";
    #ifdef __clang__
    static constexpr auto _sign_T_preambul =
            "const char *s11n::impl_s11n::_sign_T_PRETTY_FUNC() [T = ";
    #else // for gcc
        #ifdef S11N_CAN_CONSTEXPR_PRETTY_FUNC
            static constexpr auto _sign_T_preambul =
            "constexpr const char* s11n::impl_s11n::_sign_T_PRETTY_FUNC() [with T = ";
        #else
            static constexpr auto _sign_T_preambul =
            "const char* s11n::impl_s11n::_sign_T_PRETTY_FUNC() [with T = ";
        #endif
    #endif // __clang__
    //===================================================================================
    //  Возвращает указатель, следующий за паттерном.
    //  По идее, должен проверять правильность паттерна вначале. Пока не знаю как
    //  это реализовать в 11-м стандарте.
    constexpr const char* _advance_along_substr( const char* src,
                                                 const char* pattern )
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
    //===================================================================================
}} // namespace s11n::impl_s11n
//=======================================================================================

#endif // S11N_IMPL_NAME_OF_TYPE_FROM_PF_H
