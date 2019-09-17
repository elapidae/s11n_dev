#ifndef S11N_H
#define S11N_H

#include "impl/std_string.h"
#include "impl/std_list.h"

#include "impl/signature.h"

#include "impl/encode.h"
#include "impl/decode.h"


//=======================================================================================
namespace s11n
{
    //===================================================================================
    template<typename T> struct Serial;
    //===================================================================================
    template<typename T>
    std::string signature()
    {
        return impl::signature<T>();
    }
    //===================================================================================
    template<typename T> constexpr
    uint32_t signature_crc()
    {
        return impl::signature_crc<T>();
    }
    //===================================================================================
    using impl::calc_crc;   //  ( const std::string& );
    using impl::crc_type;
    //===================================================================================
    using impl::encode;     //  std::string encode( const T& val );
                            //  void encode( const T& val, Writer* writer );
    //===================================================================================
    using impl::decode;     //  T decode( Reader* reader );
                            //  T decode( const std::string& buf );
    //===================================================================================
} // namespace s11n
//=======================================================================================


#endif // S11N_H
