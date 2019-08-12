#ifndef S11N_ENCODER_H
#define S11N_ENCODER_H

#include <type_traits>
#include <string>

#include "s11n_size.h"
#include "s11n_writer.h"
#include "impl/tuple_helper.h"
#include "impl/container_helper.h"
#include "impl/name_of_type.h"
#include "impl/type_spec.h"


//=======================================================================================
namespace s11n
{
    //===================================================================================
    class Encoder
    {
    public:

        template<typename T>
        static void encode( const T& val, Writer* writer );

        template<typename T>
        static std::string encode( const T& val );


    private:

        template<typename T, impl::type_spec>
        struct _write_splitter;

        template<int idx, typename Tuple>
        struct _tuple_writer;
    }; // Encoder
    //===================================================================================


    //===================================================================================
    //      Implementation
    //===================================================================================
    template<typename T>
    std::string Encoder::encode( const T& val )
    {
        Writer w;
        encode( val, &w );
        return w.result();
    }
    //===================================================================================
    template<typename T>
    void Encoder::encode( const T& val, Writer* writer )
    {
        _write_splitter<T,impl::spec_of<T>()>::write( val, writer );
    }
    //===================================================================================


    //===================================================================================
    template<typename T>
    struct Encoder::_write_splitter<T, impl::type_spec::as_plain>
    {
        static void write( const T& val, Writer* writer )
        {
            writer->write_plain( val );
        }
    };
    //===================================================================================
    template<typename T>
    struct Encoder::_write_splitter<T, impl::type_spec::as_own_read_write>
    {
        static void write( const T& val, Writer* writer )
        {
            s11n::Serial<T>::write( val, writer );
        }
    };
    //===================================================================================
    template<typename T>
    struct Encoder::_write_splitter<T, impl::type_spec::as_serial_tuple>
    {
        static void write( const T& val, Writer* writer )
        {
            Encoder::encode( s11n::Serial<T>::to_tuple(val), writer );
        }
    };
    //===================================================================================
    template<typename T>
    struct Encoder::_write_splitter<T, impl::type_spec::as_container>
    {
        static void write( const T& val, Writer* writer )
        {
            s11n::Size sz( val.size() );
            Encoder::encode( sz, writer );

            for ( const auto & elem: val )
                Encoder::encode( elem, writer );
        }
    };
    //===================================================================================
    template<typename T>
    struct Encoder::_write_splitter<T, impl::type_spec::as_tuple>
    {
        static void write( const T& val, Writer* writer )
        {
            _tuple_writer<impl::tuple_start_idx<T>(),T>::write( val, writer );
        }
    };
    //===================================================================================
    template<typename T>
    struct Encoder::_write_splitter<T, impl::type_spec::error>
    {
        //constexpr
        static int write( const T&, Writer* )
        {
            //static_assert (false, "Cannot define specification type of type" );
            throw std::logic_error(
                        std::string("Cannot define specification of type '") +
                        impl::name_of_type<T>().str() + "'" );
            return 0;
        }
    };
    //===================================================================================


    //===================================================================================
    //  tuple writer process
    template<int idx,typename Tuple>
    struct Encoder::_tuple_writer
    {
        static void write( const Tuple& tupl, Writer* writer )
        {
            auto val = std::get<idx>( tupl );

            Encoder::encode( val, writer );

            constexpr auto next_idx = impl::tuple_next_idx<idx,Tuple>();
            _tuple_writer<next_idx,Tuple>::write( tupl, writer );
        }
    };
    //-----------------------------------------------------------------------------------
    //  tuple writer tail
    template<typename Tuple>
    struct Encoder::_tuple_writer<-1,Tuple>
    {
        static void write( const Tuple&, Writer* )
        {} // do nothing
    };
    //===================================================================================
} // namespace s11n
//=======================================================================================


#endif // S11N_ENCODER_H
