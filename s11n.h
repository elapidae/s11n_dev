#ifndef S11N_H
#define S11N_H


#include "s11n_name_of_type.h"
#include "impl/std_string.h"
#include "impl/signature.h"
#include "impl/crc.h"

namespace s11n
{
    template<typename T>
    std::string signature()
    {
        return impl::signature<T>();
    }

    template<typename T>
    uint32_t crc()
    {
        return impl::crc<T>();
    }


} // namespace s11n


#endif // S11N_H
