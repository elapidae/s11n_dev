#ifndef S11N_SIZE_H
#define S11N_SIZE_H

#include <cstddef>
#include "../s11n/impl/str_view.h"
#include "../s11n/impl/serializer.h"


namespace s11n {

class Size
{
public:
    explicit Size( uint32_t val = 0 );

    operator uint32_t() const;

private:
    uint32_t _val;
};

template<typename T> struct Serial;

template<>
struct Serial<Size>
{
    static impl::Serializer& serialize(impl::Serializer& ser, const Size& sz_)
    {
        uint32_t sz = sz_;
        while( sz )
        {
            auto b = uint8_t(sz & 0x7F);
            sz >>= 7;

            if (sz == 0)
                b |= 0x80;

            ser.put<uint8_t>( b );
        }
        return ser;
    }

    static impl::Deserializer& deserialize(impl::Deserializer& des, Size& sz_)
    {
        uint32_t res        = 0;
        int      shift      = 0;
        bool     stop       = false;
        int      pos_watch  = 0;

        do
        {
            if ( ++pos_watch > 5 )
                throw std::runtime_error( "Size out" );

            uint32_t cur = des.get<uint8_t>();
            stop = cur & 0x80;
            cur &= 0x7F;
            cur <<= shift;
            shift += 7;
            res |= cur;
        }
        while( !stop );

        sz_ = Size(res);

        return des;
    }
};

} // namespace s11n

#endif // S11N_SIZE_H
