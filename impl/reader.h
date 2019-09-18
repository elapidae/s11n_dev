#ifndef S11N_IMPL_READER_H
#define S11N_IMPL_READER_H

#include <string>
#include <stdexcept>
#include <string.h> // for memcpy
#include "impl/endian.h"

//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    class Reader
    {
    public:
        //-------------------------------------------------------------------------------
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wpragmas"
        #pragma GCC diagnostic ignored "-Wweak-vtables"

        class error : public std::runtime_error
        { public: error(const std::string& msg) : runtime_error(msg) {} };

        class out_of_buffer : public error
        { public: out_of_buffer(const std::string& msg) : error(msg) {} };

        class buffer_not_finished : public error
        { public: buffer_not_finished() : error("buffer_not_finished") {} };

        class out_of_size : public error
        { public: out_of_size(const std::string& msg) : error(msg) {} };

        #pragma GCC diagnostic pop
        //-------------------------------------------------------------------------------
        void throw_if_not_finished()
        {
            if (_remained == 0) return;
            throw buffer_not_finished();
        }
        //-------------------------------------------------------------------------------
        explicit Reader( const std::string& buffer )
            : _buffer   ( buffer          )
            , _ptr      ( _buffer.c_str() )
            , _remained ( _buffer.size()  )
        {}
        //-------------------------------------------------------------------------------
        std::string read_str( size_t sz )
        {
            if ( _remained < sz )
                throw out_of_buffer( "In Reader::read_str()" );

            std::string res( _ptr, sz );

            _ptr      += sz;
            _remained -= sz;

            return res;
        }
        //-------------------------------------------------------------------------------
        template<typename T> typename std::enable_if< std::is_arithmetic<T>::value,
        T>::type read()
        {
            if ( _remained < sizeof(T) )
                throw out_of_buffer( "In Reader::read()" );

            //  Лучше сделать сначала переменную, а не сразу "снимать" с буффера, чтобы
            //  компилятор сам подумал за выравнивание.
            //  remove_const нужно, если, например, надо выдавливать пары в std::map.
            typename std::remove_const<T>::type res;
            memcpy( &res, _ptr, sizeof(T) );

            _ptr      += sizeof(T);
            _remained -= sizeof(T);

            return impl::little_endian( res );
        }
        //-------------------------------------------------------------------------------

    private:
        std::string _buffer;
        const char* _ptr;
        size_t      _remained;
    };
    //===================================================================================
}} // namespace s11n::impl
//=======================================================================================

#endif // S11N_IMPL_READER_H
