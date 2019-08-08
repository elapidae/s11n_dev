#ifndef S11N_READER_H
#define S11N_READER_H

#include "impl/tuple_helper.h"
#include "impl/endian.h"

#include <string.h> // for memcpy

namespace s11n
{
    class Reader
    {
    public:
        explicit Reader( const std::string& buffer );

    public:
        template<typename PlainT> typename
        std::enable_if< std::is_arithmetic<PlainT>::value,PlainT>::type
        read_plain();

        std::string read_str( size_t sz );


    private:
        std::string _buffer;
        const char* _ptr;
        size_t      _remained;
    };



    template<typename PlainT> typename
    std::enable_if< std::is_arithmetic<PlainT>::value,PlainT>::type
    Reader::read_plain()
    {
        if ( _remained < sizeof(PlainT) )
            throw std::out_of_range( "In Reader::read_plain()" );

        //  Лучше сделать сначала переменную, а не сразу "снимать" с буффера, чтобы
        //  компилятор сам подумал за выравнивание.
        PlainT res;
        memcpy( &res, _ptr, sizeof(PlainT) );

        _ptr      += sizeof(PlainT);
        _remained -= sizeof(PlainT);

        return impl::little_endian( res );
    }

} // namespace s11n

#endif // S11N_READER_H
