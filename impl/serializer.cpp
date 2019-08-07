#include "serializer.h"

namespace s11n {
namespace impl {

Deserializer::Deserializer( const std::string& src )
    : _src( src )
    , _ptr( _src.c_str() )
    , _remained( _src.size() )
{}


} // namespace impl
} // namespace s11n
