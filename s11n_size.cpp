#include "s11n_size.h"

#include <assert.h>
#include <limits>

namespace s11n
{

Size::Size(uint32_t val)
    : _val( val )
{}

Size::Size(size_t val)
    : _val( uint32_t(val) )
{
    assert( val <= std::numeric_limits<uint32_t>::max() );
}

s11n::Size::operator uint32_t() const
{
    return _val;
}

} // namespace s11n
