#ifndef S11N_IMPL_SIZE_H
#define S11N_IMPL_SIZE_H

#include <assert.h>
#include <limits>
#include "impl_s11n/writer.h"
#include "impl_s11n/reader.h"


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

//        explicit Size( size_t val )
//            : _val( static_cast<uint32_t>(val) )
//        {
//            assert( val <= std::numeric_limits<uint32_t>::max() );
//        }

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
