#include "only_crc.h"

//#include "impl/tuple_helper.h"
//#include "impl/name_of_type.h"
//#include "impl/description.h"

//#include "impl/str_view.h"
//#include "impl/name_of_type_from_pf.h"
//#include "impl/std_string.h"

#include <tuple>
#include <type_traits>
#include "impl/std_void_t.h"


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
namespace s11n {
namespace impl
{
    //===================================================================================
    template<typename T> constexpr
    bool is_tuple();
    //===================================================================================
    template <typename T>
    constexpr bool has_serial_tuple();
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
    template<typename T, typename = void>
    struct _has_tuple_size
        : std::false_type
    {};
    //-----------------------------------------------------------------------------------
    template<typename T>
    struct _has_tuple_size< T, std::void_t<decltype(std::tuple_size<T>::value)> >
        : std::true_type
    {};
    //-----------------------------------------------------------------------------------
    template<typename T> constexpr
    bool is_tuple() { return _has_tuple_size<T>::value; }
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
    template <typename T>
    constexpr bool has_serial_tuple()
    {
        return _has_serial_tuple<T>::value;
    }
    //===================================================================================

    //===================================================================================
    template <typename T>
    using serial_tuple_type = decltype( s11n::Serial<T>::to_tuple(std::declval<T>()) );
    //===================================================================================
} // namespace impl
} // namespace s11n
//=======================================================================================

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
    std::ostream& operator << ( std::ostream& os, const str_view& sv );
    //===================================================================================

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
    static constexpr size_t index_of( const char *src, char pattern )
    {
        return _index_of_with_recurse( src, pattern, 0 );
    }
    //===================================================================================
    //  length -- for comfort.
    static constexpr size_t length( const char *src )
    {
        return index_of( src, '\0' );
    }
    //===================================================================================

} // namespace impl
} // namespace s11n
//=======================================================================================

//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    //  terminal_ch добавлен, чтобы в дальнейшем подцеплять имена контейнеров до их
    //  аргументов (до '<').
    template <typename T, char terminal_ch = ']'> constexpr
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
    template <typename T, char terminal_ch> constexpr
    str_view name_of_type_from_PF()
    {
        return
        {
            _pos_T_in_pattern<T>(),
            index_of( _pos_T_in_pattern<T>(), terminal_ch )
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
    //
    //===================================================================================
} // impl
} // s11n
//=======================================================================================
//      Implementation
//=======================================================================================





#include <type_traits>
#include <array>
//#include "impl/str_view.h"
//#include "impl/std_void_t.h"


//=======================================================================================
//      description: введена с целью ведения версионности, изменений типов.
//
//  Если в перегрузке есть соотвествующее поле:
//  template <>
//  struct s11n::Serial<SomeType>
//  {
//      static constexpr auto description = "some words, ver. 1";
//  };
//  , то в сигнатуру типа описание войдет в скобочках: "SomeType(some words, ver. 1)".
//  Если такого поля нету, то описания не будет.
//=======================================================================================
namespace s11n {
namespace impl
{
    template <typename T> constexpr
    impl::str_view description();

    template <typename T> constexpr
    std::array<impl::str_view,3> description_in_squares();

    template <typename T>
    std::string description_in_squares_str();

}} // s11n::impl namespaces
//=======================================================================================



//=======================================================================================
//      Implementation
//=======================================================================================
//      Need forward declaration
namespace s11n
{
    template <typename T> struct Serial;
}
//=======================================================================================

namespace s11n {
namespace impl
{
    //===================================================================================
    //      Serial<T>::description deduction
    template<typename T, typename = void>
    struct _has_serial_description
        : std::false_type
    {};
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
    //===================================================================================


    //===================================================================================
    //
    //  Без if constexpr, в 11-ом свернуть нельзя.
    template <typename T> constexpr typename
    std::enable_if< has_serial_description<T>(), impl::str_view>::type
    _description()
    {
        return
        {
            s11n::Serial<T>::description,
            length( s11n::Serial<T>::description )
        };
    }
    //===================================================================================
    template <typename T> constexpr typename
    std::enable_if< !has_serial_description<T>(), impl::str_view >::type
    _description()
    {
        return {};
    }
    //===================================================================================
    template <typename T>
    constexpr impl::str_view description()
    {
        return _description<T>();
    }
    //===================================================================================

    //===================================================================================
    template <typename T> constexpr
    std::array<impl::str_view,3> description_in_squares()
    {
        using sv = impl::str_view;
        return description<T>().len == 0
                ? std::array<sv,3>{ sv{}, sv{}, sv{} }
                : std::array<sv,3>{ sv{"(",1}, description<T>(), sv{")",1} };
    }
    //===================================================================================
    template <typename T>
    std::string description_in_squares_str()
    {
        auto d = description_in_squares<T>();
        return d[0].str() + d[1].str() + d[2].str();
    }
    //===================================================================================
} // namespace impl
} // namespace s11n
//=======================================================================================

//=======================================================================================
//      name_of_type: введен для поправки имени типа, если, по какой-то причине, с этим
//  не справился самовывод. Например, если не подправить имя для std::string, то
//  имя выводится примерно так: "std::__cxx11::basic_string<char>".
//
//  NB! Работа была проверена только на компиляторе gcc!!! Для других компиляторов
//  не знаю, но если узнаю, допилю.
//
//  Чтобы поправить имя, следует определить соответствующее статическое поле:
//  template <>
//  struct s11n::Serial<std::string>
//  {
//      static constexpr auto name_of_type = "std::string";
//  };
//
//  Если поля нету, то имя типа будет выводится:
//
//   - для арифметических типов: bool, char, intN, uintN, floatN, где N - размер в битах;
//
//   - для остальных -- через парсинг результата жизнедеятельности макроса
//     __PRETTY_FUNCTION__.
//
//=======================================================================================
namespace s11n {
namespace impl
{
    template <typename T> constexpr
    impl::str_view name_of_type();
}}
//=======================================================================================



//=======================================================================================
//      Implementation
//=======================================================================================
namespace s11n
{
    template <typename T> struct Serial;
}
//=======================================================================================
//
namespace s11n {
namespace impl
{
    //===================================================================================
    //      Plain name of type deduction
    //
    template <typename T>
    constexpr const typename std::enable_if< std::is_arithmetic<T>::value,
    impl::str_view>::type _name_of_type()
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
    //===================================================================================


    //===================================================================================
    //      Serial<T>::name_of_type deduction
    //
    template<typename T, typename = void>
    struct _has_serial_name_of_type
        : std::false_type
    {};
    //-----------------------------------------------------------------------------------
    template<typename T>
    struct _has_serial_name_of_type
            <
                T,
                typename std::void_t<decltype(s11n::Serial<T>::name_of_type)>
            >
        : std::true_type
    {};
    //-----------------------------------------------------------------------------------
    template<typename T> constexpr
    bool has_serial_name_of_type()
    {
        return _has_serial_name_of_type<T>::value;
    }
    //-----------------------------------------------------------------------------------
    template <typename T> constexpr typename
    std::enable_if
    <
        has_serial_name_of_type<T>(),
        impl::str_view
    >::type
    _name_of_type()
    {
        return
        {
            s11n::Serial<T>::name_of_type,
            length( s11n::Serial<T>::name_of_type )
        };
    }
    //===================================================================================

    //===================================================================================
    //      Elsewhere extract name_of_type from __PRETTY_FUNCTION__
    //
    template <typename T> constexpr typename
    std::enable_if
    <
        !has_serial_name_of_type<T>() && !std::is_arithmetic<T>::value,
        impl::str_view
    >::type
    _name_of_type()
    {
        return name_of_type_from_PF<T>();
    }
    //===================================================================================

    //===================================================================================
    template <typename T> constexpr
    impl::str_view name_of_type()
    {
        return _name_of_type<T>();
    }
    //===================================================================================
} // namespace impl
} // namespace s11n
//=======================================================================================



//=======================================================================================
//  Для расчета CRC использован один из стандартных полиномов: 04C11DB7.
//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    //  Считает CRC сигнатуры в compile-time. Саму сигнатуру при этом не производит.
    //  Если она будет меняться, то и этот код надо будет подправлять.
    //
    template <typename T>
    constexpr uint32_t crc();
    //===================================================================================
    //  Для проверки.
    //uint32_t poly_04C11DB7( const std::string &buf );
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
    //  Предварительное объявление обязательно, они вызывают друг друга рекурсивно.
    //
    template <typename T> constexpr typename
    std::enable_if< is_tuple<T>(), uint32_t>::type _crc(uint32_t prev);
    //
    template <typename T> constexpr typename
    std::enable_if< has_serial_tuple<T>(), uint32_t>::type _crc(uint32_t prev);
    //
    template <typename T> constexpr typename
    std::enable_if< !is_tuple<T>() &&
                    !has_serial_tuple<T>(), uint32_t>::type _crc(uint32_t prev);
    //===================================================================================


    //===================================================================================
    constexpr uint32_t _poly_04C11DB7_table[256] =
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
    //-----------------------------------------------------------------------------------
    static uint32_t poly_04C11DB7( const std::string &buf )
    {
        uint32_t crc = 0xFFFFFFFF;
        for ( auto ch: buf )
        {
            uint8_t uch  = static_cast<uint8_t>(ch);
            uint8_t ucrc = static_cast<uint8_t>(crc);
            uint8_t idx = ucrc ^ uch;
            crc = (crc >> 8) ^ _poly_04C11DB7_table[idx];
        }
        crc ^= 0xFFFFFFFF;
        return crc;
    }
    //===================================================================================

    //===================================================================================
    //  Пошаговое вычисление CRC
    constexpr uint32_t _crc_ch_step( char ch, uint32_t prev )
    {
        return (prev >> 8) ^ _poly_04C11DB7_table[ uint8_t(uint8_t(prev)^uint8_t(ch)) ];
    }
    //  Итерации внутри строки.
    constexpr uint32_t _crc_ch_recurse( const str_view& sw, uint pos, uint32_t prev )
    {
        return pos == sw.len
                    ? prev
                    : _crc_ch_recurse( sw, pos + 1, _crc_ch_step( sw.ptr[pos],prev) );
    }
    //  Считаем всю строку целиком.
    constexpr uint32_t _crc_str( const str_view& sw, uint32_t prev )
    {
        return _crc_ch_recurse( sw, 0, prev );
    }
    //
    //===================================================================================


    //===================================================================================
    //  Итерируем до тех пор, пока не достигнем последнего элемента, когда достигнем,
    //  установим индекс в -1 и выйдем через специализированную структуру.
    //
    //  Тогда как в сигнатуре закрывающую скобочку можно было подредактировать на выходе,
    //  здесь приходится считать непременно в return-e.
    //
    template<int idx, typename atuple>
    struct _tuple_crc
    {
        static constexpr uint32_t calc_crc( uint32_t prev )
        {
            using element = tuple_element<atuple,idx>;

            //  Brain collapsing with one return...
            return _tuple_crc<tuple_next_idx<idx,atuple>(),atuple>::calc_crc
            (
                _crc_ch_step
                (
                    tuple_next_idx<idx,atuple>() > 0 ? ',' : '}',
                    _crc<element>( prev )
                )
            );
        }
    };
    //-----------------------------------------------------------------------------------
    //  Скобочку опять надо "закрывать" на месте.
    template<typename atuple>
    struct _tuple_crc<-1,atuple>
    {
        static constexpr uint32_t calc_crc( uint32_t prev )
        {
            return tuple_size<atuple>() != 0
                    ? prev
                    : _crc_ch_step('}', prev);
        }
    };
    //-----------------------------------------------------------------------------------
    template<typename atuple> constexpr
    uint32_t tuple_crc( uint32_t prev )
    {
        return _tuple_crc<tuple_start_idx<atuple>(),atuple>::calc_crc
        (
            _crc_ch_step( '{', prev )
        );
    }
    //===================================================================================

    //===================================================================================
    //  Вложенный ужос, но, в 11-м Стандарте по другому никак.
    template <typename T> constexpr
    uint32_t _crc_of_description( uint32_t prev )
    {
        return
        _crc_str
        (
            description_in_squares<T>()[2], _crc_str
            (
                description_in_squares<T>()[1], _crc_str
                (
                    description_in_squares<T>()[0],
                    prev
                )
            )
        );
    }
    //===================================================================================
    template <typename T> constexpr
    uint32_t _crc_of_name( uint32_t prev )
    {
        return _crc_str( name_of_type<T>(), prev );
    }
    //===================================================================================
    template <typename T> constexpr typename
    std::enable_if< is_tuple<T>(), uint32_t>::type
    _crc( uint32_t prev )
    {
        return tuple_crc<T>( prev );
    }
    //-----------------------------------------------------------------------------------
    template <typename T> constexpr typename
    std::enable_if< has_serial_tuple<T>(), uint32_t>::type
    _crc( uint32_t prev )
    {
        return
        tuple_crc<serial_tuple_type<T>>
        (
            _crc_of_description<T>
            (
                _crc_of_name<T>( prev )
            )
        );
    }
    //-----------------------------------------------------------------------------------
    template <typename T> constexpr typename
    std::enable_if< !is_tuple<T>() &&
                    !has_serial_tuple<T>(), uint32_t>::type
    _crc( uint32_t prev )
    {
        return _crc_of_description<T>
        (
            _crc_of_name<T>( prev )
        );
    }
    //===================================================================================

    //===================================================================================
} // namespace impl
} // namespace s11n
//=======================================================================================
//      Implementation
//=======================================================================================

only_crc::only_crc()
{

}
