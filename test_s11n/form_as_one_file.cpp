#ifndef S11N_IMPL_STR_VIEW
#define S11N_IMPL_STR_VIEW

#include <string>

//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    struct str_view
    {
        const char *ptr;
        size_t len;

        std::string str() const
        {
            return {ptr, len};
        }
        operator std::string() const { return str(); }
    };
    //===================================================================================
    //  Индекс первого найденного символа в строке. Здесь вход в рекурсию.
    constexpr size_t str_index_of( const char *src, char pattern );
    //===================================================================================
    //  length -- for comfort.
    constexpr size_t str_length( const char *src );
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
    //  Индекс первого найденного символа в строке, в рекурсии.
    //  См. _index_of()
    static constexpr size_t _index_of_with_recurse( const char *src,
                                                    char pattern,
                                                    size_t accum )
    {
        //  cannot in c++11: static_assert ( *src != '\0', "" );
        return *src == pattern
                    ? accum
                    : _index_of_with_recurse( src + 1, pattern, accum + 1 );
    }
    //===================================================================================
    //  Индекс первого найденного символа в строке. Здесь вход в рекурсию.
    constexpr size_t str_index_of( const char *src, char pattern )
    {
        return _index_of_with_recurse( src, pattern, 0 );
    }
    //===================================================================================
    //  length -- for comfort.
    constexpr size_t str_length( const char *src )
    {
        return str_index_of( src, '\0' );
    }
    //===================================================================================
}} // namespace s11n::impl
//=======================================================================================

#endif // S11N_IMPL_STR_VIEW
#ifndef S11N_IMPL_STD_VOID_T_H
#define S11N_IMPL_STD_VOID_T_H

//=======================================================================================
//  Какие-то старые компиляторы дурят с типом void_t, добавлено для совместимости.
//
//  Реализация #ifndef, по-моему, кривая. Если сообразите как сделать грамотную проверку,
//  буду сильно признателен.
//=======================================================================================

#include <type_traits>


//=======================================================================================
#ifndef void_t
    namespace std
    {
        template< class... >
        using void_t = void;
    }
#endif
//=======================================================================================

#endif // S11N_IMPL_STD_VOID_T_H
#ifndef S11N_IMPL_CRC_H
#define S11N_IMPL_CRC_H



//=======================================================================================
//
//  Для расчета CRC использован один из стандартных полиномов: 04C11DB7.
//  UPD 2019-08-21 -- Расчет CRC перенесен в signature.h, здесь оставлены только
//  входные функции и предрасчетная таблица.
//
//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    using crc_type = uint32_t;

    static constexpr crc_type crc_first_val = 0xFFFFFFFF;
    static constexpr crc_type crc_last_xor  = 0xFFFFFFFF;
    //===================================================================================
    constexpr crc_type calc_crc_ch( char ch, crc_type prev );
    //===================================================================================
    constexpr crc_type calc_crc_str( const str_view& str, crc_type prev );
    //===================================================================================
    //  Для проверки.
    static crc_type calc_crc( const std::string& buf );
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
    constexpr crc_type _poly_04C11DB7_table[256] =
    {
        0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA,
        0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
        0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
        0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
        0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,
        0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
        0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
        0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
        0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
        0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
        0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940,
        0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
        0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116,
        0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
        0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
        0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
        0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A,
        0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
        0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818,
        0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
        0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
        0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
        0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C,
        0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
        0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,
        0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
        0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
        0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
        0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086,
        0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
        0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4,
        0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
        0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
        0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
        0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
        0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
        0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE,
        0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
        0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
        0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
        0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252,
        0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
        0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60,
        0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
        0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
        0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
        0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04,
        0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
        0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
        0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
        0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
        0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
        0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E,
        0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
        0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C,
        0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
        0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
        0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
        0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0,
        0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
        0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6,
        0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
        0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
        0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
    };
    //===================================================================================

    //===================================================================================
    //  Пошаговое вычисление CRC
    constexpr crc_type calc_crc_ch( char ch, crc_type prev )
    {
        return (prev >> 8) ^ _poly_04C11DB7_table[ uint8_t(uint8_t(prev)^uint8_t(ch)) ];
    }
    //-----------------------------------------------------------------------------------
    //  Итерации внутри строки.
    constexpr crc_type _crc_str_recurse( const str_view& str, uint pos, crc_type prev )
    {
        return pos == str.len
                    ? prev
                    : _crc_str_recurse( str, pos + 1, calc_crc_ch( str.ptr[pos],prev) );
    }
    //-----------------------------------------------------------------------------------
    //  Считаем всю строку целиком.
    constexpr crc_type calc_crc_str( const str_view& str, crc_type prev )
    {
        return _crc_str_recurse( str, 0, prev );
    }
    //
    //===================================================================================

    //===================================================================================
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-function"
    static crc_type calc_crc( const std::string& buf )
    {
        uint32_t crc = crc_first_val;
        for ( auto ch: buf )
        {
            uint8_t uch  = static_cast<uint8_t>(ch);
            uint8_t ucrc = static_cast<uint8_t>(crc);
            uint8_t idx = ucrc ^ uch;
            crc = (crc >> 8) ^ _poly_04C11DB7_table[idx];
        }
        crc ^= crc_last_xor;
        return crc;
    }
    #pragma GCC diagnostic pop
    //===================================================================================
}} // namespace s11n::impl
//=======================================================================================

#endif // S11N_IMPL_CRC_H
#ifndef S11N_IMPL_NAME_OF_TYPE_FROM_PF_H
#define S11N_IMPL_NAME_OF_TYPE_FROM_PF_H



//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    //  terminal_ch добавлен, чтобы в дальнейшем подцеплять имена контейнеров до их
    //  аргументов (до '<').
    template <typename T> constexpr
    str_view name_of_type_from_PF();
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

    //===================================================================================
    //  Принцип извлечения имени типа:
    //   - вызываем функцию _sign_T_PRETTY_FUNC<Type>(), которая возвращает
    //      __PRETTY_FUNCTION__, в ней, так или иначе, хранится имя типа.
    //      Дальнейшая задача -- выковырять это имя в compile-time.
    //
    template <typename T> constexpr
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
    constexpr const char * _sign_T_PRETTY_FUNC()
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
}} // namespace s11n::impl
//=======================================================================================

#endif // S11N_IMPL_NAME_OF_TYPE_FROM_PF_H
#ifndef S11N_IMPL_TUPLE_HELPER_H
#define S11N_IMPL_TUPLE_HELPER_H


#include <tuple>
#include <type_traits>


//=======================================================================================
//      serial tuple
//
//      Чтобы можно было сериализовывать и десериализовывать объекты, был выбран путь
//  соспоставления кортежа с каким-либо типом. Для бысторого внедрения типа достаточно
//  специализировать структуру s11n::Serial, в которой создать статический метод
//  std::tuple<args..> to_tuple(Type);
//
//  Например:
//  namespace s11n
//  {
//      template<>
//      struct Serial<std::chrono::seconds>
//      {
//          static std::tuple<int64_t> to_tuple( const std::chrono::seconds& sec )
//          {
//              return std::make_tuple( sec.count() );
//          }
//      };
//  }
//
//  Порядок аргументов достжен соответствовать хотя бы одному конструктору объекта, тогда
//  декодер сможет инстанцировать объект.
//
//=======================================================================================
namespace s11n { template<typename> struct Serial; }

namespace s11n {
namespace impl
{
    //===================================================================================
    template<typename T> constexpr
    bool is_tuple();
    //===================================================================================
    template<typename T> constexpr
    bool has_serial_tuple();
    //===================================================================================
    template<typename T>
    using serial_tuple_type = decltype(s11n::Serial<T>::to_tuple(std::declval<T>()));
    //===================================================================================

    //===================================================================================
    template<typename T> constexpr
    int tuple_size();
    //-----------------------------------------------------------------------------------
    template<typename T, int idx>
    using tuple_element = typename std::tuple_element<idx,T>::type;
    //-----------------------------------------------------------------------------------
    template<typename T> constexpr
    int tuple_start_idx();
    //-----------------------------------------------------------------------------------
    template<int idx, typename T> constexpr
    int tuple_next_idx();
    //===================================================================================
}}
//=======================================================================================


//=======================================================================================
//      Implementation
//=======================================================================================
namespace s11n { template <typename> struct Serial; }


//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    //      is_tuple<T>() use as marker with signatures.
    //
    //-----------------------------------------------------------------------------------
    template<typename T> constexpr
    bool _is_array( const T* )
    {
        return false;
    }
    //-----------------------------------------------------------------------------------
    template<typename T, size_t sz> constexpr
    bool _is_array( const std::array<T,sz>* )
    {
        return true;
    }
    //-----------------------------------------------------------------------------------
    template<typename T, typename = void>
    struct _has_tuple_size
        : std::false_type
    {};
    //-----------------------------------------------------------------------------------
    template<typename T>
    struct _has_tuple_size< T, std::void_t< decltype(std::tuple_size<T>::value) >>
        : std::true_type
    {};
    //-----------------------------------------------------------------------------------
    template<typename T> constexpr
    bool is_tuple()
    {
        return _has_tuple_size<T>::value &&
               !_is_array( static_cast<const T*>(nullptr) );
    }
    //===================================================================================


    //===================================================================================
    //      for simplify...
    //
    template<typename T> constexpr
    int tuple_size() { return std::tuple_size<T>::value; }
    //===================================================================================


    //===================================================================================
    //      Use for recurse tuple unpacking.
    //      Используется при итерациях с кортежами.
    //
    template<typename T> constexpr
    int tuple_start_idx() { return std::tuple_size<T>::value == 0 ? -1 : 0; }
    //-----------------------------------------------------------------------------------
    template<int idx, typename T> constexpr
    int tuple_next_idx()  { return idx + 1 == tuple_size<T>() ? -1 : idx + 1; }
    //===================================================================================


    //===================================================================================
    template <typename T, typename = void>
    struct _has_serial_tuple
        : std::false_type
    {};
    //-----------------------------------------------------------------------------------
    template <typename T>
    struct _has_serial_tuple
    <
        T,
        std::void_t< decltype(s11n::Serial<T>::to_tuple) >
    >
        : std::true_type
    {};
    //-----------------------------------------------------------------------------------
    //  Проверка, что to_tuple() возвращает какой-то кортеж.
    template<typename T> constexpr typename std::enable_if<
    _has_serial_tuple<T>::value, bool>::type
    _serial_method_is_tuple()
    {
        return is_tuple<serial_tuple_type<T>>();
    }
    //-----------------------------------------------------------------------------------
    template<typename T> constexpr typename std::enable_if<
    !_has_serial_tuple<T>::value, bool>::type
    _serial_method_is_tuple()
    {
        return false;
    }
    //-----------------------------------------------------------------------------------
    template <typename T>
    constexpr bool has_serial_tuple()
    {
        static_assert ( !_has_serial_tuple<T>::value ||
                        !std::is_arithmetic<T>::value,
                        "Arithmetic types cannot have Serial<T>::to_tuple()." );

        static_assert( !_has_serial_tuple<T>::value ||
                       _serial_method_is_tuple<T>(),
                       "Serial<T>::to_tuple() must return std::tuple only." );

        return _has_serial_tuple<T>::value;
    }
    //===================================================================================
} // namespace impl
} // namespace s11n
//=======================================================================================

#endif // S11N_IMPL_TUPLE_HELPER_H
#ifndef S11N_IMPL_CONTAINER_HELPER
#define S11N_IMPL_CONTAINER_HELPER

#include <utility>      // for std::forward
#include <type_traits>


//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    template<typename C> constexpr
    bool is_container();
    //===================================================================================
    template<typename C> constexpr
    bool is_map_set();
    //===================================================================================
    template<typename Cont, typename Value>
    typename std::enable_if<!is_map_set<Cont>(), void>::type
    container_append( Cont* c, Value && val );
    //-----------------------------------------------------------------------------------
    template<typename Cont, typename Value>
    typename std::enable_if<is_map_set<Cont>(), void>::type
    container_append( Cont* c, Value && val );
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
    template<typename C, typename = void>
    struct _is_container
        : public std::false_type
    {};
    //-----------------------------------------------------------------------------------
    template<typename C>
    struct _is_container
    <
        C,
        std::void_t
        <
            decltype(std::declval<C>().begin()),
            decltype(std::declval<C>().end()),
            decltype(std::declval<C>().size()),
            typename C::value_type
        >
    >
        : public std::true_type
    {};
    //-----------------------------------------------------------------------------------
    template<typename C> constexpr
    bool is_container()
    {
        return _is_container<C>::value;
    }
    //===================================================================================

    //===================================================================================
    template<typename C, typename = void>
    struct _is_map_set
        : public std::false_type
    {};
    //-----------------------------------------------------------------------------------
    template<typename C>
    struct _is_map_set
    <
        C,
        std::void_t
        <
            typename C::value_type,
            typename C::key_type,
            decltype( std::declval<C>().insert
                        (
                            *static_cast<typename C::value_type*>(nullptr)
                        )
                    )
        >
    >
        : public std::true_type
    {};
    //-----------------------------------------------------------------------------------
    template<typename C> constexpr
    bool is_map_set()
    {
        return _is_map_set<C>::value;
    }
    //===================================================================================

    //===================================================================================
    template<typename Cont, typename Value>
    typename std::enable_if<!is_map_set<Cont>(), void>::type
    container_append( Cont* c, Value && val )
    {
        c->push_back( std::forward<Value>(val) );
    }
    //===================================================================================
    template<typename Cont, typename Value>
    typename std::enable_if<is_map_set<Cont>(), void>::type
    container_append( Cont* c, Value && val )
    {
        c->insert( std::forward<Value>(val) );
    }
    //===================================================================================
} // namespace impl
} // namespace s11n
//=======================================================================================

#endif // S11N_IMPL_CONTAINER_HELPER
#ifndef S11N_IMPL_METAARGS_HELPER_H
#define S11N_IMPL_METAARGS_HELPER_H


//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    template< template<typename...> class MetaT, typename ... Args >
    std::string signature_metaargs( const MetaT<Args...>* );
    //-----------------------------------------------------------------------------------
    template< template<typename...> class MetaT, typename ... Args >
    constexpr crc_type calc_crc_metaargs( const MetaT<Args...>*, crc_type prev );
    //===================================================================================
}} // namespace s11n::impl
//=======================================================================================



//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    template<typename T>
    std::string signature();
    //===================================================================================
    template<typename T> constexpr
    crc_type calc_crc_T( crc_type prev );
    //===================================================================================


    //===================================================================================
    //  signature of args
    template< typename ... Args >
    struct _signature_metaargs;
    //-----------------------------------------------------------------------------------
    template< typename T1, typename ... Args >
    struct _signature_metaargs<T1,Args...>
    {
        //-------------------------------------------------------------------------------
        static std::string sign( bool first = true )
        {
            return std::string(first ? "" : ",") +
                   impl::signature<T1>() +
                   _signature_metaargs<Args...>::sign( false );
        }
        //-------------------------------------------------------------------------------
        static constexpr crc_type crc( crc_type prev, bool first = true )
        {
            return
                _signature_metaargs<Args...>::crc
                (
                    calc_crc_T<T1>
                    (
                        first ? prev : calc_crc_ch( ',', prev )
                    ),
                    false // not first
                );
        }
        //-------------------------------------------------------------------------------
    };
    //-----------------------------------------------------------------------------------
    template<>
    struct _signature_metaargs<>
    {
        //-------------------------------------------------------------------------------
        static std::string sign( bool first = true )
        {
            (void) first;
            return {};
        }
        //-------------------------------------------------------------------------------
        static constexpr crc_type crc( crc_type prev, bool /*first*/ = true )
        {
            return prev;
        }
        //-------------------------------------------------------------------------------
    };
    //===================================================================================
    template< template<typename...> class MetaT, typename ... Args >
    std::string signature_metaargs( const MetaT<Args...>* )
    {
        return _signature_metaargs<Args...>::sign();
    }
    //-----------------------------------------------------------------------------------
    template< template<typename...> class MetaT, typename ... Args >
    constexpr crc_type calc_crc_metaargs( const MetaT<Args...>*, crc_type prev )
    {
        return _signature_metaargs<Args...>::crc( prev );
    }
    //  signature of args
    //===================================================================================
}} // namespace s11n::impl
//=======================================================================================

#endif // S11N_IMPL_METAARGS_HELPER_H
#ifndef S11N_IMPL_SIGNATURE_1_NAME_OF_TYPE_H
#define S11N_IMPL_SIGNATURE_1_NAME_OF_TYPE_H



//=======================================================================================
//  signature_1:
//      if type is arithmetic: using plain name such as uint32
//      if type has Serial<T>::name_of_type specification: using this spec
//      if type is tuple: using {T1,T2,...,TN} spec
//      elsewhere using name of type from __PRETTY_FUNCTION__
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
namespace s11n { template <typename T> struct Serial; }
//===================================================================================
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
        as_tuple,
        as_pretty_func
    };
    //-----------------------------------------------------------------------------------
    template <typename T> constexpr
    sign_spec_1 sign_spec_1_of()
    {
        return    std::is_arithmetic<T>::value        ? sign_spec_1::as_plain
                : impl::has_serial_name_of_type<T>()  ? sign_spec_1::as_own_name_of_type
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
                                                   impl::str_view{"", 0};
        }
        //-------------------------------------------------------------------------------
        static std::string sign()
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
                ::s11n::Serial<T>::name_of_type,
                str_length( ::s11n::Serial<T>::name_of_type )
            };
        }
        //-------------------------------------------------------------------------------
        static std::string sign()
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
    //  as_tuple
    template <typename T>
    struct _signature_1<T, impl::sign_spec_1::as_tuple>
    {
        //-------------------------------------------------------------------------------
        static std::string sign()
        {
            return std::string("{") +
                   signature_metaargs(static_cast<T*>(nullptr)) +
                   std::string("}");
        }
        //-------------------------------------------------------------------------------
        static constexpr crc_type crc( crc_type prev )
        {
            return  calc_crc_ch( '}',
                        calc_crc_metaargs(static_cast<T*>(nullptr),
                            calc_crc_ch( '{', prev )
                        )
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
        static std::string sign()
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
        return _signature_1<T,sign_spec_1_of<T>()>::sign();
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
#ifndef S11N_IMPL_SIGNATURE_2_METAARGS_H
#define S11N_IMPL_SIGNATURE_2_METAARGS_H



//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    template <typename T>
    std::string signature_2();
    //===================================================================================
    template <typename T>
    constexpr crc_type calc_crc_2( crc_type prev );
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
    //  signature_2: signature_1 + metaargs
    //===================================================================================
    enum class sign_spec_2
    {
        without_metatype,
        as_tuple,
        as_container,
        as_any_metatype,
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
    sign_spec_2 sign_spec_2_of()
    {
        return is_tuple<T>()          ? sign_spec_2::as_tuple
            : impl::is_container<T>() ? sign_spec_2::as_container
            : impl::is_metatype<T>()  ? sign_spec_2::as_any_metatype
                                      : sign_spec_2::without_metatype;
    }
    //===================================================================================
    template <typename T, impl::sign_spec_2>
    struct _signature_2;
    //===================================================================================
    //  as_tuple
    template <typename T>
    struct _signature_2<T, impl::sign_spec_2::as_tuple>
    {
        //-------------------------------------------------------------------------------
        static std::string sign()
        {
            return {};
        }
        //-------------------------------------------------------------------------------
        static constexpr crc_type crc( crc_type prev )
        {
            return prev;
        }
        //-------------------------------------------------------------------------------
    };
    //  as_tuple
    //===================================================================================
    //  as_container
    template <typename T>
    struct _signature_2<T, impl::sign_spec_2::as_container>
    {
        //-------------------------------------------------------------------------------
        using _vt = typename T::value_type;
        //-------------------------------------------------------------------------------
        static std::string sign()
        {
            return std::string("<") +
                   impl::signature<_vt>() +
                   std::string(">");
        }
        //-------------------------------------------------------------------------------
        static constexpr crc_type crc( crc_type prev )
        {
            return calc_crc_ch( '>',
                        calc_crc_T<_vt>(
                            calc_crc_ch( '<', prev )
                        )
                    );
        }
        //-------------------------------------------------------------------------------
    };
    //  as_container
    //===================================================================================
    //  as_any_metatype
    //-----------------------------------------------------------------------------------
    //  _signature_2
    template <typename T>
    struct _signature_2<T, impl::sign_spec_2::as_any_metatype>
    {
        //-------------------------------------------------------------------------------
        static std::string sign()
        {
            return std::string("<") +
                   signature_metaargs( static_cast<T*>(nullptr) ) +
                   std::string(">");
        }
        //-------------------------------------------------------------------------------
        static constexpr crc_type crc( crc_type prev )
        {
            return  calc_crc_ch( '>',
                        calc_crc_metaargs(static_cast<T*>(nullptr),
                            calc_crc_ch( '<', prev )
                        )
                    );
        }
        //-------------------------------------------------------------------------------
    };
    //  as_any_metatype
    //===================================================================================
    //  without_metatype
    template <typename T>
    struct _signature_2<T, impl::sign_spec_2::without_metatype>
    {
        //-------------------------------------------------------------------------------
        static std::string sign()
        {
            return {};
        }
        //-------------------------------------------------------------------------------
        static constexpr crc_type crc( crc_type prev )
        {
            return prev;
        }
        //-------------------------------------------------------------------------------
    };
    //  without_metatype
    //===================================================================================
    template <typename T>
    std::string signature_2()
    {
        return signature_1<T>() +
              _signature_2<T,sign_spec_2_of<T>()>::sign();
    }
    //-----------------------------------------------------------------------------------
    template <typename T>
    constexpr crc_type calc_crc_2( crc_type prev )
    {
        return _signature_2<T,sign_spec_2_of<T>()>::crc
               (
                    calc_crc_1<T>( prev )
               );
    }
    //===================================================================================
    //      signature_2
    //===================================================================================
}} // namespace s11n::impl
//=======================================================================================

#endif // S11N_IMPL_SIGNATURE_2_METAARGS_H
#ifndef S11N_IMPL_SIGNATURE_3_DESCRIPTION_H
#define S11N_IMPL_SIGNATURE_3_DESCRIPTION_H



//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    template <typename T>
    std::string signature_3();
    //===================================================================================
    template <typename T>
    constexpr crc_type calc_crc_3( crc_type prev );
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
    //      signature_3 = signature_2 + description
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
        static_assert ( !_has_serial_description<T>::value ||
                        !std::is_arithmetic<T>::value,
                        "Arithmetic types cannot have description." );

        return _has_serial_description<T>::value;
    }
    //      Serial<T>::description deduction
    //===================================================================================
    enum class sign_spec_3
    {
        with_description,
        without_description
    };
    //-----------------------------------------------------------------------------------
    template<typename T>
    constexpr sign_spec_3 sign_spec_3_of()
    {
        return  has_serial_description<T>() ? sign_spec_3::with_description
                                            : sign_spec_3::without_description;
    }
    //===================================================================================
    template<typename T, sign_spec_3>
    struct _signature_3;
    //===================================================================================
    template<typename T>
    struct _signature_3<T,sign_spec_3::without_description>
    {
        //-------------------------------------------------------------------------------
        static constexpr crc_type crc( crc_type prev )
        {
            return prev;
        }
        //-------------------------------------------------------------------------------
        static std::string sign()
        {
            return {};
        }
        //-------------------------------------------------------------------------------
    };
    //===================================================================================
    template<typename T>
    struct _signature_3<T,sign_spec_3::with_description>
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
        static constexpr crc_type crc( crc_type prev )
        {
            return calc_crc_ch
                    (
                        ')',
                        calc_crc_str
                        (
                            description(),
                            calc_crc_ch( '(', prev )
                        )
                    );
        }
        //-------------------------------------------------------------------------------
        static std::string sign()
        {
            return std::string("(") + description().str() + ')';
        }
        //-------------------------------------------------------------------------------
    };
    //-----------------------------------------------------------------------------------
    template <typename T>
    std::string signature_3()
    {
        return signature_2<T>() +
              _signature_3<T,sign_spec_3_of<T>()>::sign();
    }
    //-----------------------------------------------------------------------------------
    template <typename T>
    constexpr crc_type calc_crc_3( crc_type prev )
    {
        return
            _signature_3<T,sign_spec_3_of<T>()>::crc
            (
                calc_crc_2<T>( prev )
            );
    }
    //===================================================================================
    //      signature_3 = signature_2 + description
    //===================================================================================
}} // namespace s11n::impl
//=======================================================================================


#endif // S11N_IMPL_SIGNATURE_3_DESCRIPTION_H
#ifndef S11N_IMPL_SIGNATURE_4_SERIAL_TUPLE_H
#define S11N_IMPL_SIGNATURE_4_SERIAL_TUPLE_H



//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    template <typename T>
    std::string signature_4();
    //===================================================================================
    template <typename T>
    constexpr crc_type calc_crc_4( crc_type prev );
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
    //      signature_3 = signature_2 + serial tuple specialization
    //===================================================================================
    enum class sign_spec_4
    {
        with_serial_tuple,
        without_serial_tuple
    };
    //-----------------------------------------------------------------------------------
    template <typename T>
    constexpr sign_spec_4 sign_spec_4_of()
    {
        return has_serial_tuple<T>() ? sign_spec_4::with_serial_tuple
                                     : sign_spec_4::without_serial_tuple;
    }
    //===================================================================================
    template<typename T,sign_spec_4>
    struct _signature_4;
    //-----------------------------------------------------------------------------------
    template<typename T>
    struct _signature_4<T,sign_spec_4::without_serial_tuple>
    {
        //-------------------------------------------------------------------------------
        static constexpr crc_type crc( crc_type prev )
        {
            return prev;
        }
        //-------------------------------------------------------------------------------
        static std::string sign()
        {
            return {};
        }
        //-------------------------------------------------------------------------------
    };
    //-----------------------------------------------------------------------------------
    template<typename T>
    struct _signature_4<T,sign_spec_4::with_serial_tuple>
    {
        //-------------------------------------------------------------------------------
        using Tup = decltype( s11n::Serial<T>::to_tuple(std::declval<T>()) );
        //-------------------------------------------------------------------------------
        static constexpr crc_type crc( crc_type prev )
        {
            return calc_crc_T<Tup>( prev );
        }
        //-------------------------------------------------------------------------------
        static std::string sign()
        {
            return impl::signature<Tup>();
        }
        //-------------------------------------------------------------------------------
    };
    //-----------------------------------------------------------------------------------
    template <typename T>
    std::string signature_4()
    {
        return signature_3<T>() +
              _signature_4<T,sign_spec_4_of<T>()>::sign();
    }
    //-----------------------------------------------------------------------------------
    template <typename T>
    constexpr crc_type calc_crc_4( crc_type prev )
    {
        return _signature_4<T,sign_spec_4_of<T>()>::crc
                (
                    calc_crc_3<T>( prev )
                );
    }
    //===================================================================================
    //      signature_4 = signature_3 + serial tuple specialization
    //===================================================================================
} // namespace impl
} // namespace s11n
//=======================================================================================

#endif // S11N_IMPL_SIGNATURE_4_SERIAL_TUPLE_H
#ifndef S11N_IMPL_SIGNATURE_H
#define S11N_IMPL_SIGNATURE_H


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
    crc_type signature_crc();
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
    //      Result wrap
    //===================================================================================
    template <typename T> constexpr
    bool test_type()
    {
        static_assert ( !std::is_pointer<T>::value,
                        "s11n::signature don't work with pointers." );
        static_assert ( !std::is_member_pointer<T>::value,
                        "s11n::signature don't work with member pointers." );
        static_assert ( !std::is_member_function_pointer<T>::value,
                        "s11n::signature don't work with member function pointers." );
        static_assert ( !std::is_function<T>::value,
                        "s11n::signature don't work with functions." );
        static_assert ( !std::is_array<T>::value,
                        "s11n::signature don't work with arrays. "
                        "Use std::array instead." );
        return true;
    }
    //===================================================================================
    template <typename T>
    std::string signature()
    {
        static_assert( test_type<T>(), "");
        return signature_4<T>();
    }
    //-----------------------------------------------------------------------------------
    template <typename T> constexpr
    crc_type calc_crc_T( crc_type prev )
    {
        static_assert( test_type<T>(), "");
        return calc_crc_4<T>( prev );
    }
    //-----------------------------------------------------------------------------------
    template <typename T> constexpr
    crc_type signature_crc()
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
#ifndef S11N_IMPL_ENDIAN_H
#define S11N_IMPL_ENDIAN_H

#include <endian.h>
#include <type_traits>


//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    //  Функция выворачивает число наизнанку, если архитектура big_endian.
    template <typename T> typename std::enable_if<std::is_arithmetic<T>::value,T>::type
    little_endian( T val );
    //===================================================================================
} // namespace impl
} // namespace s11n
//=======================================================================================


//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    //  Функция выворачивает число наизнанку, если архитектура big_endian.
    template <typename T> typename std::enable_if<std::is_arithmetic<T>::value,T>::type
    little_endian( T val )
    {
        static_assert( sizeof(T) == 1 || sizeof(T) == 2 ||
                       sizeof(T) == 4 || sizeof(T) == 8,
                       "Strange size of T, must be 1 or 2 or 4 or 8." );

        static_assert ( BYTE_ORDER == LITTLE_ENDIAN || BYTE_ORDER == BIG_ENDIAN,
                        "Cannot define endian." );

      #if BYTE_ORDER == LITTLE_ENDIAN
        // nothing to do...
      #elif BYTE_ORDER == BIG_ENDIAN
        auto *ch = static_cast<char*>( static_cast<void*>(&val) );
        constexpr auto tsize = sizeof(T);

        switch( tsize )
        {
        case 8: std::swap( ch[3], ch[tsize-4] );
                std::swap( ch[2], ch[tsize-3] );
        case 4: std::swap( ch[1], ch[tsize-2] );
        case 2: std::swap( ch[0], ch[tsize-1] );
        }
      #else
        #error "Unknown byte order";
      #endif

        return val;
    }
    //===================================================================================
} // namespace impl
} // namespace s11n
//=======================================================================================

#endif // S11N_IMPL_ENDIAN_H
#ifndef S11N_IMPL_READER_H
#define S11N_IMPL_READER_H

#include <string>
#include <stdexcept>
#include <string.h> // for memcpy

//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    class Reader
    {
    public:
        //-------------------------------------------------------------------------------
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wpragmas"
        #pragma GCC diagnostic ignored "-Wweak-vtables"

        class error : public std::runtime_error
        { public: error(const std::string& msg) : runtime_error(msg) {} };

        class out_of_buffer : public error
        { public: out_of_buffer(const std::string& msg) : error(msg) {} };

        class buffer_not_finished : public error
        { public: buffer_not_finished() : error("buffer_not_finished") {} };

        class out_of_size : public error
        { public: out_of_size(const std::string& msg) : error(msg) {} };

        #pragma GCC diagnostic pop
        //-------------------------------------------------------------------------------
        void throw_if_not_finished()
        {
            if (_remained == 0) return;
            throw buffer_not_finished();
        }
        //-------------------------------------------------------------------------------
        explicit Reader( const std::string& buffer )
            : _buffer   ( buffer          )
            , _ptr      ( _buffer.c_str() )
            , _remained ( _buffer.size()  )
        {}
        //-------------------------------------------------------------------------------
        std::string read_str( size_t sz )
        {
            if ( _remained < sz )
                throw out_of_buffer( "In Reader::read_str()" );

            std::string res( _ptr, sz );

            _ptr      += sz;
            _remained -= sz;

            return res;
        }
        //-------------------------------------------------------------------------------
        template<typename T> typename std::enable_if< std::is_arithmetic<T>::value,
        T>::type read()
        {
            if ( _remained < sizeof(T) )
                throw out_of_buffer( "In Reader::read()" );

            //  Лучше сделать сначала переменную, а не сразу "снимать" с буффера, чтобы
            //  компилятор сам подумал за выравнивание.
            //  remove_const нужно, если, например, надо выдавливать пары в std::map.
            typename std::remove_const<T>::type res;
            memcpy( &res, _ptr, sizeof(T) );

            _ptr      += sizeof(T);
            _remained -= sizeof(T);

            return impl::little_endian( res );
        }
        //-------------------------------------------------------------------------------

    private:
        std::string _buffer;
        const char* _ptr;
        size_t      _remained;
    };
    //===================================================================================
} // namespace impl
} // namespace s11n
//=======================================================================================

#endif // S11N_IMPL_READER_H
#ifndef S11N_IMPL_WRITER_H
#define S11N_IMPL_WRITER_H

#include <string>


//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    class Writer
    {
    public:
        template<typename T> typename std::enable_if< std::is_arithmetic<T>::value,
        void>::type write(T val);

        void write_str( const std::string& app )        { _buffer.append(app); }

        const std::string& result() const               { return _buffer; }

    private:
        std::string _buffer;
    };
    //===================================================================================
}} // namespace s11n::impl
//=======================================================================================


//=======================================================================================
//      Implementation
//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    template<typename T> typename std::enable_if< std::is_arithmetic<T>::value,
    void>::type Writer::write( T val )
    {
        val = impl::little_endian( val );

        auto *ch = static_cast<char*>( static_cast<void*>(&val) );

        _buffer.append( ch, sizeof(val) );
    }
    //===================================================================================
}} // namespace s11n::impl
//=======================================================================================

#endif // S11N_IMPL_WRITER_H
#ifndef S11N_IMPL_TYPE_SPEC_H
#define S11N_IMPL_TYPE_SPEC_H


//=======================================================================================
//      Специализация типа нужна, чтобы специализироваться на Encoder/Decoder.
//  Действует в приоритетном порядке:
//      as_plain,
//      as_own_read_write,
//      as_serial_tuple,
//      as_container,
//      as_tuple;
//  Т.е. даже если специализировать, например, тип int, то он все равно будет считаться
//  as_plain, ибо нефиг.
//
//  Таким образом, можно специализировать свои read|write для какого-нибудь контейнера,
//  но нельзя прострелить ногу с plain типами.
//
//  Опять же, можно сделать свои read/write, но тогда нет смысла делать to_tuple().
//=======================================================================================


//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    enum class type_spec { as_plain,
                           as_own_read_write,
                           as_serial_tuple,
                           as_container,
                           as_tuple,
                           error
                          };
    //===================================================================================
    template<typename T> constexpr
    type_spec type_spec_of();
    //===================================================================================
}} // s11n::impl namespaces
//=======================================================================================


//=======================================================================================
//  Implementation:
//      _has_serial_read<T>()
//      _has_serial_write<T>()
//      has_serial_read_write<T>()
//      type_spec_of<T>()
//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    //      Has Serial<T>::write
    //===================================================================================
    template<typename SerT, typename = void>
    struct _s_has_serial_write : public std::false_type {};
    //-----------------------------------------------------------------------------------
    template<typename SerT>
    struct _s_has_serial_write
        <
            SerT,
            std::void_t
            <
                decltype(s11n::Serial<SerT>::write(std::declval<SerT>(),nullptr))
            >
        > : public std::true_type {};
    //-----------------------------------------------------------------------------------
    template<typename SerT> constexpr
    bool _has_serial_write()
    {
        return _s_has_serial_write<SerT>::value;
    }
    //===================================================================================
    //      Has Serial<T>::write
    //===================================================================================


    //===================================================================================
    //      Has Serial<T>::read
    //===================================================================================
    template<typename SerT, typename = void>
    struct _s_has_serial_read : public std::false_type {};
    //-----------------------------------------------------------------------------------
    template<typename SerT>
    struct _s_has_serial_read
        <
            SerT,
            std::void_t
            <
                decltype( s11n::Serial<SerT>::read(nullptr) )
            >
        > : public std::true_type {};
    //-----------------------------------------------------------------------------------
    template<typename SerT> constexpr
    bool _has_serial_read()
    {
        return _s_has_serial_read<SerT>::value;
    }
    //===================================================================================
    //      Has Serial<T>::read
    //===================================================================================


    //===================================================================================
    //      Has Serial<T>::read && write
    //===================================================================================
    template<typename SerT> constexpr
    bool _has_serial_read_write()
    {
        static_assert( (!_has_serial_read<SerT>() && !_has_serial_write<SerT>()) ||
                       ( _has_serial_read<SerT>() &&  _has_serial_write<SerT>()),
                        "Define both Serial<T>::read && write" );

        return _has_serial_read<SerT>() && _has_serial_write<SerT>();
    }
    //===================================================================================
    template<typename SerT> constexpr
    bool has_serial_read_write()
    {
        //  Могут быть определены либо read|write, либо to_tuple, но не одновременно.
        static_assert( ( !_has_serial_read_write<SerT>() && !has_serial_tuple<SerT>()) ||
                       (  _has_serial_read_write<SerT>() && !has_serial_tuple<SerT>()) ||
                       ( !_has_serial_read_write<SerT>() &&  has_serial_tuple<SerT>()),
                        "Define Serial<T>::read && write "
                        "or define to_tuple, but not both." );

        return _has_serial_read_write<SerT>();
    }
    //===================================================================================
    //      Has Serial<T>::read && write
    //===================================================================================


    //===================================================================================
    template<typename T> constexpr
    type_spec _type_spec_of()
    {
        return std::is_arithmetic<T>::value     ? type_spec::as_plain
             : impl::has_serial_read_write<T>() ? type_spec::as_own_read_write
             : impl::has_serial_tuple<T>()      ? type_spec::as_serial_tuple
             : impl::is_container<T>()          ? type_spec::as_container
             : impl::is_tuple<T>()              ? type_spec::as_tuple
                                                : type_spec::error;
    }
    //===================================================================================
    template<typename T> constexpr
    type_spec type_spec_of()
    {
        static_assert( _type_spec_of<T>() != type_spec::error,
                       "Cannot define spec of type" );

        return _type_spec_of<T>();
    }
    //===================================================================================
} // namespace impl
} // namespace s11n
//=======================================================================================

#endif // S11N_IMPL_TYPE_SPEC_H
#ifndef S11N_IMPL_SIZE_H
#define S11N_IMPL_SIZE_H

#include <assert.h>
#include <limits>


//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    class Size
    {
    public:
        explicit Size( uint32_t val = 0 )
            : _val( val )
        {}

        explicit Size( size_t val = 0 )
            : _val( static_cast<uint32_t>(val) )
        {
            assert( val <= std::numeric_limits<uint32_t>::max() );
        }

        operator uint32_t() const { return _val; }

    private:
        uint32_t _val;
    };
    //===================================================================================
} // namespace impl
} // namespace s11n
//=======================================================================================


//=======================================================================================
namespace s11n
{
    //===================================================================================
    template<typename T> struct Serial;
    //===================================================================================

    //===================================================================================
    template<>
    struct Serial< impl::Size >
    {
        //===============================================================================
        static void write( const impl::Size& sz_, impl::Writer* w )
        {
            uint32_t sz = sz_;
            do
            {
                auto b = uint8_t(sz & 0x7F);
                sz >>= 7;

                if (sz == 0)
                    b |= 0x80;

                w->write<uint8_t>( b );
            }
            while( sz );
        } // write
        //===============================================================================

        //===============================================================================
        static impl::Size read( impl::Reader* reader )
        {
            uint32_t res        = 0;
            int      shift      = 0;
            bool     stop       = false;
            int      pos_watch  = 0;

            do
            {
                if ( ++pos_watch > 5 )
                    throw impl::Reader::out_of_size( "Size out" );

                uint32_t cur = reader->read<uint8_t>();
                stop = cur & 0x80;
                cur &= 0x7F;
                cur <<= shift;
                shift += 7;
                res |= cur;
            }
            while( !stop );

            return impl::Size{res};
        } // read
        //===============================================================================
    }; // struct Serial< impl::Size >
    //===================================================================================
} // namespace s11n
//=======================================================================================


#endif // S11N_IMPL_SIZE_H
#ifndef S11N_IMPL_ENCODE_H
#define S11N_IMPL_ENCODE_H


//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    template<typename T>
    std::string encode( const T& val );
    //-----------------------------------------------------------------------------------
    template<typename T>
    void encode( const T& val, Writer* writer );
    //===================================================================================
}} // namespace s11n::impl
//=======================================================================================


//=======================================================================================
//      Implementation
//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    template<typename T>
    std::string encode( const T& val );
    //-----------------------------------------------------------------------------------
    template<typename T>
    void encode( const T& val, Writer* writer );
    //===================================================================================


    //===================================================================================
    template<typename T, impl::type_spec>
    struct _write_splitter;
    //===================================================================================
    template<int idx, typename Tuple>
    struct _tuple_writer;
    //===================================================================================


    //===================================================================================
    template<typename T>
    struct _write_splitter<T, impl::type_spec::as_plain>
    {
        static void write( const T& val, Writer* writer )
        {
            writer->write( val );
        }
    };
    //-----------------------------------------------------------------------------------
    template<typename T>
    struct _write_splitter<T, impl::type_spec::as_own_read_write>
    {
        static void write( const T& val, Writer* writer )
        {
            s11n::Serial<T>::write( val, writer );
        }
    };
    //-----------------------------------------------------------------------------------
    template<typename T>
    struct _write_splitter<T, impl::type_spec::as_serial_tuple>
    {
        static void write( const T& val, Writer* writer )
        {
            encode( s11n::Serial<T>::to_tuple(val), writer );
        }
    };
    //-----------------------------------------------------------------------------------
    template<typename T>
    struct _write_splitter<T, impl::type_spec::as_container>
    {
        static void write( const T& val, Writer* writer )
        {            
            encode( Size(val.size()), writer );

            for ( const auto & elem: val )
                encode( elem, writer );
        }
    };
    //-----------------------------------------------------------------------------------
    template<typename T>
    struct _write_splitter<T, impl::type_spec::as_tuple>
    {
        static void write( const T& val, Writer* writer )
        {
            _tuple_writer<impl::tuple_start_idx<T>(),T>::write( val, writer );
        }
    };
    //===================================================================================


    //===================================================================================
    //  tuple writer process
    template<int idx,typename Tuple>
    struct _tuple_writer
    {
        static void write( const Tuple& tupl, Writer* writer )
        {
            auto val = std::get<idx>( tupl );

            encode( val, writer );

            constexpr auto next_idx = impl::tuple_next_idx<idx,Tuple>();
            _tuple_writer<next_idx,Tuple>::write( tupl, writer );
        }
    };
    //-----------------------------------------------------------------------------------
    //  tuple writer tail
    template<typename Tuple>
    struct _tuple_writer<-1,Tuple>
    {
        static void write( const Tuple&, Writer* )
        {} // do nothing
    };
    //===================================================================================


    //===================================================================================
    template<typename T>
    std::string encode( const T& val )
    {
        Writer w;
        encode( val, &w );
        return w.result();
    }
    //-----------------------------------------------------------------------------------
    template<typename T>
    void encode( const T& val, Writer* writer )
    {
        _write_splitter<T,impl::type_spec_of<T>()>::write( val, writer );
    }
    //===================================================================================
}} // namespace s11n::impl
//=======================================================================================

#endif // S11N_IMPL_ENCODE_H
#ifndef S11N_IMPL_DECODE_H
#define S11N_IMPL_DECODE_H



//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    template<typename T>
    T decode( Reader* reader );
    //-----------------------------------------------------------------------------------
    template<typename T>
    T decode( const std::string& buf );
    //===================================================================================
}} // namespace s11n::impl
//=======================================================================================


//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    template<int idx, typename Res, typename Tuple, typename ... Es>
    struct _tuple_reader;
    //-----------------------------------------------------------------------------------
    //  NB! Здесь просится type folding!!!
    //  tuple reader -- tail, собственно ради этой строчки все и затевалось :).
    template< typename Tuple, typename Res, typename ... Es >
    struct _tuple_reader<-1, Tuple, Res, Es... >
    {
        static Res read( Reader*, Es&& ... es )
        {
            return Res{ std::forward<Es>(es)... };
        }
    };
    //-----------------------------------------------------------------------------------
    //  tuple reader -- process
    template< int idx, typename Tuple, typename Res, typename ... Es >
    struct _tuple_reader
    {
        static Res read( Reader* reader, Es&& ... es )
        {
            constexpr auto next_idx = impl::tuple_next_idx<idx,Tuple>();
            using val_type = impl::tuple_element<Tuple,idx>;
            using next = _tuple_reader<next_idx, Tuple, Res, Es..., val_type>;

            auto val = decode<val_type>( reader );

            return next::read( reader, std::forward<Es>(es)..., std::move(val) );
        }
    };
    //  tuple reader
    //===================================================================================


    //===================================================================================
    //  reader joiner
    template<typename T, impl::type_spec>
    struct _read_joiner;
    //-----------------------------------------------------------------------------------
    template<typename T>
    struct _read_joiner<T,impl::type_spec::as_plain>
    {
        static T read( Reader* reader )
        {
            return reader->read<T>();
        }
    };
    //-----------------------------------------------------------------------------------
    template<typename T>
    struct _read_joiner<T,impl::type_spec::as_own_read_write>
    {
        static T read( Reader* reader )
        {
            return s11n::Serial<T>::read( reader );
        }
    };
    //-----------------------------------------------------------------------------------
    template<typename T>
    struct _read_joiner<T,impl::type_spec::as_serial_tuple>
    {
        static T read( Reader* reader )
        {
            using Tuple = impl::serial_tuple_type<T>;
            constexpr auto idx = impl::tuple_start_idx<Tuple>();
            return _tuple_reader<idx,Tuple,T>::read( reader );
        }
    };
    //-----------------------------------------------------------------------------------
    template<typename T>
    struct _read_joiner<T,impl::type_spec::as_container>
    {
        static T read( Reader* reader )
        {
            size_t sz = decode<Size>( reader );

            T res;  // Can reserve needed size;
            using elem_type = typename T::value_type;
            while ( sz-- )
                impl::container_append( &res, std::move(decode<elem_type>(reader)) );

            return res;
        }
    };
    //-----------------------------------------------------------------------------------
    template<typename T>
    struct _read_joiner<T,impl::type_spec::as_tuple>
    {
        static T read( Reader* reader )
        {
            constexpr auto idx = impl::tuple_start_idx<T>();
            return _tuple_reader<idx,T,T>::read( reader );
        }
    };
    //  _read_joiner
    //===================================================================================


    //===================================================================================
    //  decode
    template<typename T>
    T decode( Reader* reader )
    {
        return _read_joiner<T, impl::type_spec_of<T>()>::read( reader );
    }
    //-----------------------------------------------------------------------------------
    template<typename T>
    T decode( const std::string& buf )
    {
        Reader reader( buf );
        auto res = decode<T>( &reader );

        reader.throw_if_not_finished();

        return res;
    }
    //  decode
    //===================================================================================
}} // namespace s11n::impl
//=======================================================================================

#endif // S11N_IMPL_DECODE_H
#ifndef S11N_IMPL_STD_LIST_H
#define S11N_IMPL_STD_LIST_H

#include <list>

//=======================================================================================
//  У списка в 11-ом G++ название плохо выводится, проще самим специфицировать.
//vdeb << signature< list<bool> >(); // std::__cxx11::list<bool>
namespace s11n
{
    template<typename T> struct Serial;

    template<typename T>
    struct Serial<std::list<T>>
    {
        static constexpr auto name_of_type = "std::list";
    };
}
//=======================================================================================

#endif // S11N_IMPL_STD_LIST_H
#ifndef S11N_IMPL_STD_STRING_H
#define S11N_IMPL_STD_STRING_H

#include <string>


//=======================================================================================
//  По умолчанию перегружен Serial<std::string>, иначе выведется истинный тип строки:
//  std::__cxx11::basic_string<char>
//=======================================================================================
namespace s11n
{
    template<typename> struct Serial;

    template<>
    struct Serial< std::string >
    {
        static constexpr auto name_of_type = "std::string";

        static void write( const std::string& str, impl::Writer* writer )
        {
            using namespace s11n::impl;
            encode( Size(str.size()), writer );
            writer->write_str( str );
        }

        static std::string read( impl::Reader* reader )
        {
            using namespace s11n::impl;
            auto size = decode<Size>( reader );
            return reader->read_str( size );
        }
    }; // Serial< std::string >
} // s11n namespace
//=======================================================================================

#endif // S11N_IMPL_STD_STRING_H
#ifndef S11N_H
#define S11N_H





//=======================================================================================
namespace s11n
{
    //===================================================================================
    template<typename T> struct Serial;    
    //===================================================================================
    template<typename T>
    std::string signature()
    {
        return impl::signature<T>();
    }
    //===================================================================================
    template<typename T> constexpr
    uint32_t signature_crc()
    {
        return impl::signature_crc<T>();
    }
    //===================================================================================
    using impl::calc_crc;   //( const std::string& );
    using impl::crc_type;
    //===================================================================================
    using impl::encode;     //  std::string encode( const T& val );
                            //  void encode( const T& val, Writer* writer );
    //===================================================================================
    using impl::decode;     //  T decode( Reader* reader );
                            //  T decode( const std::string& buf );
    //===================================================================================
} // namespace s11n
//=======================================================================================


#endif // S11N_H



//==================================================
//==================================================
#include <iostream>

int main() {
    std::cout << s11n::signature_crc<int32_t>() << ' '
              << std::hex << s11n::signature_crc<int32_t>() << std::endl;
}
//==================================================
//==================================================
