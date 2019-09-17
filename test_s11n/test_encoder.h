#ifndef TEST_ENCODER_H
#define TEST_ENCODER_H

#include "test_main.h"

using namespace s11n;
using namespace std;

//=======================================================================================
static constexpr auto _hex = "0123456789abcdef";
static std::string to_hex( std::string h )
{
    std::string res;
    for (char c: h)
    {
        uint8_t u = uint8_t(c);
        res += _hex[u >> 4];
        res += _hex[u & 0xF];
        res += ' ';
    }
    if (!res.empty()) res.pop_back(); // remove last space.
    return res;
}
//=======================================================================================
static int ch_from_hex(char ch)
{
    if (ch >= '0' && ch <= '9') return ch - '0';
    if (ch >= 'A' && ch <= 'F') return 10 + ch - 'A';
    if (ch >= 'a' && ch <= 'f') return 10 + ch - 'a';
    return -1;
}
//---------------------------------------------------------------------------------------
static std::string from_hex( const std::string& src )
{
    std::string res;
    res.resize( (src.size() + 1)/2 );

    auto cur = res.rbegin();
    size_t real_res_size = 0;

    bool in_char = false;
    for ( auto src_it = src.rbegin(); src_it != src.rend(); ++src_it )
    {
        int tmp = ch_from_hex( *src_it );
        if (tmp < 0) continue;

        if ( in_char )
        {
            *cur++ |= tmp << 4;
        }
        else
        {
            *cur = static_cast<char>( tmp );
            ++real_res_size;
        }
        in_char = !in_char;
    }

    return res.erase( 0, res.size() - real_res_size );
}
//=======================================================================================


//=======================================================================================
TEST_F( Test_s11n, plain_encode )
{
    EXPECT_EQ( s11n::encode(char('1')), "1" );

    // аккуратненько надо бы, может и сломаться.
    EXPECT_EQ( s11n::encode(true),  std::string("\1") );
    EXPECT_EQ( s11n::encode(false), std::string(1, '\0') );

    EXPECT_EQ( to_hex(encode(uint16_t(0x1234))), "34 12" );
    EXPECT_EQ( to_hex(encode(int64_t(0x1234567812345678))), "78 56 34 12 78 56 34 12" );

    tuple<uint16_t,char> t1{0x0123, '9'};
    EXPECT_EQ( to_hex(encode(t1)), "23 01 39" );
    EXPECT_EQ( t1, decode<decltype(t1)>(encode(t1)) );

    EXPECT_EQ( to_hex(encode(string("1234"))), "84 31 32 33 34" );
    EXPECT_EQ( decode<string>(from_hex("84 31 32 33 34")), "1234" );
}
//=======================================================================================


//=======================================================================================
TEST_F( Test_s11n, size_encode )
{
    s11n::impl::Size sz( 42u );
    auto str = encode( sz );

    auto sz2 = decode<impl::Size>( str );
    EXPECT_EQ( sz, sz2 );

    // 0x2a + 0x80 as terminal
    EXPECT_EQ( to_hex(str), "aa" );
}
//=======================================================================================


//=======================================================================================
struct Own
{
    int32_t a;
    uint16_t b;
    char c;
    bool d;
    s11n::impl::Size sz;

    bool operator == ( const Own& rhs ) const
    {
        return  a  == rhs.a &&
                b  == rhs.b &&
                c  == rhs.c &&
                d  == rhs.d &&
                sz == rhs.sz;
    }
    bool operator != ( const Own& rhs ) const
    {
        return !(*this == rhs);
    }
};
namespace s11n
{
    template <typename> struct Serial;
    template <> struct Serial<Own>
    {
        static std::tuple<int32_t, uint16_t, char, bool, s11n::impl::Size>
        to_tuple( const Own& own )
        {
            return make_tuple( own.a, own.b, own.c, own.d, own.sz );
        }
    };
} // s11n

TEST_F( Test_s11n, own_encode )
{
    Own o{-42, 43, 'c', true, impl::Size{12345u}};

    auto str = encode( o );
    EXPECT_EQ( to_hex(str), "d6 ff ff ff 2b 00 63 01 39 e0" );

    auto o2 = decode<Own>( str );
    EXPECT_EQ( o, o2 );
}
//=======================================================================================


//=======================================================================================
TEST_F( Test_s11n, container_encode )
{
    std::list<int> l{1,2,3,4,5,6,7,8,9,10};
    auto str = encode( l );
    auto l2 = decode<std::list<int>>( str );
    EXPECT_EQ( l, l2 );

    std::map<int,int> m;
    for (int i = 0; i < 10000; ++i)
        m.emplace( rand(), rand() );

    str = encode( m );
    auto m2 = decode<decltype(m)>( str );
    EXPECT_EQ( m, m2 );

    vector<string> v{"ololo", "alala", "hello", "", "world!"};
    str = encode(v);
    EXPECT_EQ( v, decode<vector<string>>(str) );
}
//=======================================================================================


#endif // TEST_ENCODER_H
