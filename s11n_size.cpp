#include "s11n_size.h"



namespace s11n
{

Size::Size(uint32_t val)
    : _val( val )
{}

s11n::Size::operator uint32_t() const
{
    return _val;
}

} // namespace s11n
