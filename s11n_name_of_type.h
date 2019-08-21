#ifndef S11N_NAME_OF_TYPE_H
#define S11N_NAME_OF_TYPE_H


#include "impl/name_of_type.h"


//=======================================================================================
namespace s11n
{
    //===================================================================================
    template<typename T>
    std::string name_of_type()
    {
        return impl::name_of_type<T>();
    };
    //===================================================================================

    //===================================================================================
} // namespace s11n
//=======================================================================================


#endif // S11N_NAME_OF_TYPE_H