#ifndef S11N_IMPL_ENCODE_H
#define S11N_IMPL_ENCODE_H

#include "impl_s11n/writer.h"
#include "impl_s11n/type_spec.h"
#include "impl_s11n/size.h"

//=======================================================================================
namespace s11n {
namespace impl_s11n
{
    //===================================================================================
    template<typename T>
    std::string encode( const T& val );
    //-----------------------------------------------------------------------------------
    template<typename T>
    void encode( const T& val, Writer* writer );
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
    template<typename T>
    std::string encode( const T& val );
    //-----------------------------------------------------------------------------------
    template<typename T>
    void encode( const T& val, Writer* writer );
    //===================================================================================


    //===================================================================================
    template<typename T, type_spec>
    struct _write_splitter;
    //===================================================================================
    template<int idx, typename Tuple>
    struct _tuple_writer;
    //===================================================================================


    //===================================================================================
    template<typename T>
    struct _write_splitter<T, type_spec::as_plain>
    {
        static void write( const T& val, Writer* writer )
        {
            writer->write( val );
        }
    };
    //-----------------------------------------------------------------------------------
    template<typename T>
    struct _write_splitter<T, type_spec::as_own_read_write>
    {
        static void write( const T& val, Writer* writer )
        {
            s11n::Serial<T>::write( val, writer );
        }
    };
    //-----------------------------------------------------------------------------------
    template<typename T>
    struct _write_splitter<T, type_spec::as_serial_tuple>
    {
        static void write( const T& val, Writer* writer )
        {
            encode( s11n::Serial<T>::to_tuple(val), writer );
        }
    };
    //-----------------------------------------------------------------------------------
    template<typename T>
    struct _write_splitter<T, type_spec::as_container>
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
    struct _write_splitter<T, type_spec::as_tuple>
    {
        static void write( const T& val, Writer* writer )
        {
            _tuple_writer<tuple_start_idx<T>(),T>::write( val, writer );
        }
    };
    //-----------------------------------------------------------------------------------
    template<typename T>
    struct _write_splitter<T, type_spec::error>
    {
        static void write( const T& val, Writer* writer )
        {
            //static_assert(false, "type does not defined...");
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

            constexpr auto next_idx = tuple_next_idx<idx,Tuple>();
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
        _write_splitter<T,type_spec_of<T>()>::write( val, writer );
    }
    //===================================================================================
}} // namespace s11n::impl_s11n
//=======================================================================================

#endif // S11N_IMPL_ENCODE_H
