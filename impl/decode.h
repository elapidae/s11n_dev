#ifndef S11N_IMPL_DECODE_H
#define S11N_IMPL_DECODE_H

#include "impl/reader.h"
#include "impl/type_spec.h"
#include "impl/size.h"


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
