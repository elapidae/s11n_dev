#ifndef S11N_DECODER_H
#define S11N_DECODER_H

#include <assert.h>
#include <functional>
#include <stdexcept>
#include "s11n_size.h"
#include "s11n_reader.h"
#include "impl/type_spec.h"

template<class> class TD;
template<int i> class ID;

namespace s11n
{
    //===================================================================================
    class AbstractDecoder
    {
    public:

        class DecodeError : public std::runtime_error
        { public: DecodeError(const std::string& msg); };

        template<typename T>
        static T decode( Reader* reader );

        template<typename T>
        static T decode( const std::string& buf );

        void process( const std::string& buf );

        virtual void process( Reader* buf ) = 0;
        virtual ~AbstractDecoder() = default;

    private:
        template<typename T, impl::type_spec>
        struct _read_joiner;

        template<int idx, typename Res, typename Tuple, typename ... Es>
        struct _tuple_reader;

    }; // AbstractDecoder class
    //===================================================================================
    template<typename T>
    class Decoder : public AbstractDecoder
    {
    public:
        //  Кто будет принимать сообщения.
        using ProcessFunc = std::function<void(T&&)>;

        explicit Decoder( ProcessFunc rfunc );

        virtual void process( Reader* reader ) override;

    private:
        ProcessFunc _rfunc;
    };
    //===================================================================================
    //      IMPLEMENTATION
    //===================================================================================
    //    template<typename T>
    //    Decoder<T>::Decoder( ProcessFunc rfunc )
    //        : AbstractDecoder( hash_of_name<T>(), hash_of_signature<T>() )
    //        , _rfunc(rfunc)
    //    {}
    //
    //    template<typename T>
    //    void Decoder<T>::process( ReadBuffer* buf )
    //    {
    //        _rfunc( std::move(decode<T>(buf)) );
    //    }
    template<typename T>
    Decoder<T>::Decoder(Decoder::ProcessFunc rfunc)
        : _rfunc( rfunc )
    {}
    //-----------------------------------------------------------------------------------
    template<typename T>
    void Decoder<T>::process( Reader * reader )
    {
        _rfunc( std::move(decode<T>(reader)) );
    }
    //===================================================================================
    template<typename T>
    T AbstractDecoder::decode( Reader* reader )
    {
        return _read_joiner<T, impl::type_spec_of<T>()>::read( reader );
    }
    //-----------------------------------------------------------------------------------
    template<typename T>
    T AbstractDecoder::decode( const std::string& buf )
    {
        Reader reader( buf );
        auto res = decode<T>( &reader );
        // May check remained bytes in reader. If != 0 =>as error...
        return res;
    }
    //===================================================================================

    //===================================================================================
    //  NB! Здесь просится type folding!!!
    //  tuple reader -- tail, собственно ради этой строчки все и затевалось :).
    template< typename Tuple, typename Res, typename ... Es >
    struct AbstractDecoder::_tuple_reader<-1, Tuple, Res, Es... >
    {
        static Res read( Reader*, Es&& ... es )
        {
            return Res{ std::forward<Es>(es)... };
        }
    };
    //-----------------------------------------------------------------------------------
    //  tuple reader -- process
    template< int idx, typename Tuple, typename Res, typename ... Es >
    struct AbstractDecoder::_tuple_reader
    {
        static Res read( Reader* reader, Es&& ... es )
        {
            constexpr auto next_idx = impl::tuple_next_idx<idx,Tuple>();
            using val_type = impl::tuple_element<Tuple,idx>;
            using next = _tuple_reader<next_idx, Tuple, Res, Es..., val_type>;

            auto val = AbstractDecoder::decode<val_type>( reader );

            return next::read( reader, std::forward<Es>(es)..., std::move(val) );
        }
    };
    //===================================================================================

    //===================================================================================
    //  reader joiner
    template<typename T>
    struct AbstractDecoder::_read_joiner<T,impl::type_spec::as_plain>
    {
        static T read( Reader* reader )
        {
            return reader->read_plain<T>();
        }
    };
    //-----------------------------------------------------------------------------------
    template<typename T>
    struct AbstractDecoder::_read_joiner<T,impl::type_spec::as_own_read_write>
    {
        static T read( Reader* reader )
        {
            return s11n::Serial<T>::read( reader );
        }
    };
    //-----------------------------------------------------------------------------------
    template<typename T>
    struct AbstractDecoder::_read_joiner<T,impl::type_spec::as_serial_tuple>
    {
        static T read( Reader* reader )
        {
            using Tuple = impl::serial_tuple_type<T>;
            auto idx = impl::tuple_start_idx<T>();
            return _tuple_reader<idx,Tuple,T>::read( reader );
        }
    };
    //-----------------------------------------------------------------------------------
    template<typename T>
    struct AbstractDecoder::_read_joiner<T,impl::type_spec::as_container>
    {
        static T read( Reader* reader )
        {
            using AD = AbstractDecoder;

            size_t sz = AD::decode<s11n::Size>( reader );

            T res;  // Can reserve needed size;
            using elem_type = typename T::value_type;
            while ( sz-- )
                impl::container_append( &res, std::move(AD::decode<elem_type>(reader)) );

            return res;
        }
    };
    //-----------------------------------------------------------------------------------
    template<typename T>
    struct AbstractDecoder::_read_joiner<T,impl::type_spec::as_tuple>
    {
        static T read( Reader* reader )
        {
            constexpr auto idx = impl::tuple_start_idx<T>();
            return _tuple_reader<idx,T,T>::read( reader );
        }
    };
    //-----------------------------------------------------------------------------------
    template<typename T>
    struct AbstractDecoder::_read_joiner<T,impl::type_spec::error>
    {
        static T read( Reader* )
        {
            assert( false );
        }
    };
    //-----------------------------------------------------------------------------------
    //  reader array
//    template<typename T>
//    struct AbstractDecoder::_reader<T,Type_Spec::Array>
//    {
//        static T read( ReadBuffer* rbuf )
//        {
//            using elem_type = typename T::value_type;
//            using reader = _reader< elem_type, type_spec<elem_type>() >;

//            T res;
//            for ( auto& v: res )
//                v = reader::read( rbuf );

//            return res;
//        }
//    };
//    //-----------------------------------------------------------------------------------
//    //  reader vector
//    template<typename T>
//    struct AbstractDecoder::_reader<T,Type_Spec::Vector>
//    {
//        static T read( ReadBuffer* rbuf )
//        {
//            using elem_type = typename T::value_type;
//            using reader = _reader< elem_type, type_spec<elem_type>() >;

//            auto sz = rbuf->read_size();
//            T res(sz);
//            for ( auto& v: res )
//                v = reader::read( rbuf );

//            return res;
//        }
//    };
//    //-----------------------------------------------------------------------------------
//    //  reader serialized
//    template<typename T>
//    struct AbstractDecoder::_reader<T,Type_Spec::Serialized>
//    {
//        static T read( ReadBuffer* rbuf )
//        {
//            return AbstractDecoder::decode<T>( rbuf );
//        }
//    };
//    //-----------------------------------------------------------------------------------
//    //  reader serialized size
//    template<typename T>
//    struct AbstractDecoder::_reader<T,Type_Spec::Serialized_Size>
//    {
//        static T read( ReadBuffer* rbuf )
//        {
//            return T( rbuf->read_size() );
//        }
//    };
//    //-----------------------------------------------------------------------------------
//    //  reader tuple
//    template<typename T>
//    struct AbstractDecoder::_reader<T,Type_Spec::Tuple>
//    {
//        static T read( ReadBuffer* rbuf )
//        {
//            constexpr auto idx = tuple_start_idx<T>();
//            return AbstractDecoder::_tuple_reader<idx,T,T>::read( rbuf );
//        }
//    };
//    //===================================================================================
} // namespace s11n
//=======================================================================================

#endif // S11N_DECODER_H
