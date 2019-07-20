#include "s11n_serial.h"

namespace s11n
{

std::string ct_string_view::str() const
{
    return {ptr, len};
}

} // namespace s11n
