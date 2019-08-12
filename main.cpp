#include <iostream>

#include "vlog.h"
#include "verror.h"
#include "vcat_containers.h"
#include "s11n_serial.h"
#include "s11n_size.h"
#include "vstring.h"
#include "impl/description.h"
#include "impl/name_of_type.h"
#include "impl/tuple_helper.h"
#include "impl/signature.h"

#include "impl/crc.h"

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <assert.h>

#include "s11n_encoder.h"
#include "s11n_decoder.h"

static void print_plain_name_of_type();

template <typename> class TD;

using namespace s11n::impl;


struct AAA {};
struct BBB {};
struct CCC {};

using T1 = std::tuple<char,int,long>;
using T2 = std::tuple<T1,double>;
using T3 = std::tuple<T1,T2,BBB,CCC>;
using T4 = std::tuple<T1,T2,T3>;
using T5 = std::tuple<T1,T2,T3,T4>;


struct DDD {};

namespace s11n
{
    template <> struct Serial<AAA>
    {
        static std::tuple<> to_tuple(AAA) { return {}; }
    };

    template <> struct Serial<BBB>
    {
        static std::tuple<> to_tuple(BBB) { return {}; }
        static constexpr auto name_of_type = "BBB with name of type";
    };

    template <> struct Serial<CCC>
    {
        static std::tuple<> to_tuple(CCC) { return {}; }
        static constexpr auto description = "CCC with description, ver. 1";
    };

    template <> struct Serial<DDD>
    {
        static std::tuple<float,T4> to_tuple( const DDD & )
        {
            return std::tuple<float,T4>();
        }

    };
}



namespace s11n
{
//    template <typename T>
//    struct Serial< std::vector<T> >
//    {
//        static void serialize( impl::Serializer* ser, const std::vector<T>& vec )
//        {
//            Serial<Size>::serialize( ser, Size(vec.size()) );
//            for (auto && val: vec) ;
//                //encode vector!!!
//            //ser->put( str );
//        }

//        static std::string deserialize(impl::Deserializer* des)
//        {
//            auto sz = Serial<Size>::deserialize( des );
//            return des->get_str( sz );
//        }
//    };

    template<>
    struct Serial<std::string>
    {
//        static constexpr auto name_of_type = "std::string";

//        static void serialize( impl::Serializer* ser, const std::string& str )
//        {
//            Serial<Size>::serialize( ser, Size(str.size()) );
//            ser->put( str );
//        }

//        static std::string deserialize(impl::Deserializer* des)
//        {
//            auto sz = Serial<Size>::deserialize( des );
//            return des->get_str( sz );
//        }
    };
}


struct CRC_Check
{};

namespace s11n
{
    template <>
    struct Serial<CRC_Check>
    {
        static constexpr auto name_of_type = "123456789";
    };
}


template <typename T>
void print_T_crc()
{
    auto c1 = crc<T>();
    auto c2 = poly_04C11DB7( signature<T>() );

    vdeb.hex() << "CRC:" << c1 << c2 << signature<T>();
    assert(c1 == c2);
}



template <typename T, typename = void>
struct _has_value_type : public std::false_type
{};

template <typename T>
struct _has_value_type<T, std::void_t<typename T::value_type> >
    : public std::true_type
{};

template <typename T>
static constexpr bool has_value_type()
{
    return _has_value_type<T>::value;
}


template <typename T, typename = void>
struct _has_mapped_type : public std::false_type
{};

template <typename T>
struct _has_mapped_type<T, std::void_t<typename T::mapped_type> >
    : public std::true_type
{};

template <typename T>
constexpr bool has_mapped_type()
{
    return _has_mapped_type<T>::value;
}




using namespace std;

template<class> class TD;

int main()
{
    vdeb << s11n::impl::name_of_type<std::vector<int>>();
    return 0;
    vector<int> vi;

    vdeb << _has_mapped_type< int >::value;
    vdeb << _has_mapped_type< decltype(vi) >::value;
    vdeb << _has_mapped_type< set<char> >::value;
    vdeb << _has_mapped_type< map<int,int> >::value;

    vdeb << has_mapped_type<int>();
    vdeb << has_mapped_type< decltype(vi)>();
    vdeb << has_mapped_type< set<char>>();
    vdeb << has_mapped_type< map<int,int>>();
    return 0;

    vdeb << s11n::Encoder::encode(420).size();
    vdeb << s11n::Encoder::encode( std::tuple<>{} ).size();
    vdeb << s11n::Encoder::encode( std::tuple<int>{'Q'} );

    vdeb << s11n::Encoder::encode(T1()).size();
    vdeb << s11n::Encoder::encode(T2()).size();
    vdeb << s11n::Encoder::encode(T3()).size();
    vdeb << s11n::Encoder::encode(T4()).size();
    vdeb << s11n::Encoder::encode(T5()).size();

    vdeb << s11n::Encoder::encode(std::string(40,'c'));
    vdeb << s11n::Encoder::encode(std::vector<int16_t>(40,0x4142));

    vdeb << s11n::Encoder::encode(std::map<int16_t, long>()).size();
    vdeb << name_of_type<std::map<int16_t,long>>();

    vdeb << s11n::Encoder::encode( std::pair<const int,int>(0x31323335,0x36373839) );

    std::vector<int> vi1{1,2,3,4,5};
    auto dvi = s11n::Encoder::encode( vi1 );
    auto vi2 = s11n::AbstractDecoder::decode<decltype(vi1)>( dvi );
    assert( vi1 == vi2);

    T1 t1{1,2,3};
    auto buf = s11n::Encoder::encode(t1);
    auto t2 = s11n::AbstractDecoder::decode<decltype(t1)>( buf );
    assert( t1 == t2 );

    return 0;

    print_T_crc<std::string>();

    print_T_crc<CRC_Check>();
    print_T_crc<int>();
    print_T_crc<AAA>();
    print_T_crc<BBB>();
    print_T_crc<CCC>();
    print_T_crc<std::tuple<int>>();
    print_T_crc<std::tuple<>>();
    print_T_crc<T4>();
    print_T_crc<DDD>();

    vdeb << signature<std::string>();
    vdeb << signature<std::unordered_map<int,int>>();
    vdeb << signature<std::unordered_set<int>>();

    vdeb << "============ has serial tuple:";
    vdeb << has_serial_tuple<int>();
    vdeb << has_serial_tuple<AAA>();
    vdeb << has_serial_tuple<BBB>();
    vdeb << has_serial_tuple<CCC>();

    vdeb << "============ is_tuple:";
    auto t = std::tuple<char,int,long>();
    vdeb << "t is tuple:" << is_tuple<decltype(t)>();
    vdeb << "bool is tuple:" << is_tuple<bool>();

    vdeb << "============ name of special types:";
    vdeb << name_of_type<AAA>();
    vdeb << name_of_type<BBB>();

    vdeb << "============ Descriptions in squares:";
    vdeb << description_in_squares_str<int>();
    vdeb << description_in_squares_str<AAA>();
    vdeb << description_in_squares_str<BBB>();
    vdeb << description_in_squares_str<CCC>();

    print_plain_name_of_type();

    return 0;
}



static void print_plain_name_of_type()
{
    using namespace s11n::impl;

    vdeb << "============ names of plain types:";
    vdeb << name_of_type<char>();
    vdeb << name_of_type<bool>();
    vdeb;
    vdeb << name_of_type<int8_t>();
    vdeb << name_of_type<int16_t>();
    vdeb << name_of_type<int32_t>();
    vdeb << name_of_type<int64_t>();
    vdeb;
    vdeb << name_of_type<uint8_t>();
    vdeb << name_of_type<uint16_t>();
    vdeb << name_of_type<uint32_t>();
    vdeb << name_of_type<uint64_t>();
    vdeb;
    vdeb << name_of_type<float>();
    vdeb << name_of_type<double>();
}




