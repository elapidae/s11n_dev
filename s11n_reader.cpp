#include "s11n_reader.h"

using namespace s11n;

Reader::Reader( const std::string& buffer )
    : _buffer   ( buffer          )
    , _ptr      ( _buffer.c_str() )
    , _remained ( _buffer.size()  )
{}

std::string Reader::read_str( size_t sz )
{
    if ( _remained < sz )
        throw std::out_of_range( "In Reader::read_str()" );

    std::string res( _ptr, sz );

    _ptr      += sz;
    _remained -= sz;

    return res;
}

