#ifndef S11N_IMPL_WRITER_H
#define S11N_IMPL_WRITER_H

#include <string>
#include "impl/endian.h"


//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    class Writer
    {
    public:
        template<typename T> typename std::enable_if< std::is_arithmetic<T>::value,
        void>::type write(T val);

        void write_str( const std::string& app )        { _buffer.append(app); }

        const std::string& result() const               { return _buffer; }

    private:
        std::string _buffer;
    };
    //===================================================================================
}} // namespace s11n::impl
//=======================================================================================


//=======================================================================================
//      Implementation
//=======================================================================================
namespace s11n {
namespace impl
{
    //===================================================================================
    template<typename T> typename std::enable_if< std::is_arithmetic<T>::value,
    void>::type Writer::write( T val )
    {
        val = impl::little_endian( val );

        auto *ch = static_cast<char*>( static_cast<void*>(&val) );

        _buffer.append( ch, sizeof(val) );
    }
    //===================================================================================
}} // namespace s11n::impl
//=======================================================================================

#endif // S11N_IMPL_WRITER_H
