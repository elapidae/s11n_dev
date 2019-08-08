#ifndef S11N_WRITER_H
#define S11N_WRITER_H

#include <type_traits>
#include <string>
#include "impl/endian.h"


namespace s11n
{

    class Writer
    {
    public:
        template<typename PlainT> typename
        std::enable_if< std::is_arithmetic<PlainT>::value,void>::type
        write_plain(PlainT val);

        void write_str( const std::string& str );

        const std::string& result() const;

    private:
        std::string _buffer;
    };

} // namespace s11n



//      Implementation
namespace s11n
{
    template<typename> struct Serial;

    template<typename PlainT> typename
    std::enable_if< std::is_arithmetic<PlainT>::value,void>::type
    Writer::write_plain(PlainT val)
    {
        val = impl::little_endian( val );
        auto *ch = static_cast<char*>( static_cast<void*>(&val) );
        _buffer.append( ch, sizeof(val) );
    }
}

namespace s11n {
namespace impl
{

    template<typename SerT, typename = void>
    struct _has_serial_write : public std::false_type {};

    template<typename SerT>
    struct _has_serial_write
        <
            SerT,
            std::void_t
            <   // фейкрвый вызов для определения наличия write с соотв. сигнатурой.
                decltype( s11n::Serial<SerT>::write(std::declval<SerT>(),nullptr) )
            >
        >
        : public std::true_type
    {};

    template<typename SerT> constexpr
    bool has_serial_write()
    {
        return _has_serial_write<SerT>::value;
    }

} // impl namespace
} // s11n namespace



#endif // S11N_WRITER_H
