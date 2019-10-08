#ifndef S11N_IMPL_SIZE_H
#define S11N_IMPL_SIZE_H

#include <assert.h>
#include <limits>
#include "impl_s11n/writer.h"
#include "impl_s11n/reader.h"


//=======================================================================================
namespace s11n {
namespace impl_s11n
{
    //===================================================================================
    class Size
    {
    public:
        explicit Size( uint32_t val = 0 )
            : _val( val )
        {}

        operator uint32_t() const { return _val; }

    private:
        uint32_t _val;
    };
    //===================================================================================
}} // namespace s11n::impl_s11n
//=======================================================================================


//=======================================================================================
namespace s11n
{
    //===================================================================================
    template<typename T> struct Serial;
    //===================================================================================

    //===================================================================================
    template<>
    struct Serial< impl_s11n::Size >
    {
        //===============================================================================
        static void write( const impl_s11n::Size& sz_, impl_s11n::Writer* w )
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
        static impl_s11n::Size read( impl_s11n::Reader* reader )
        {
            uint32_t res        = 0;
            int      shift      = 0;
            bool     stop       = false;
            int      pos_watch  = 0;

            do
            {
                if ( ++pos_watch > 5 )
                    throw impl_s11n::Reader::out_of_size( "Size out" );

                uint32_t cur = reader->read<uint8_t>();
                stop = cur & 0x80;
                cur &= 0x7F;
                cur <<= shift;
                shift += 7;
                res |= cur;
            }
            while( !stop );

            return impl_s11n::Size{res};
        } // read
        //===============================================================================
    }; // struct Serial< impl_s11n::Size >
    //===================================================================================
} // namespace s11n
//=======================================================================================


#endif // S11N_IMPL_SIZE_H
