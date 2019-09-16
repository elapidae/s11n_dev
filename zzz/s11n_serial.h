#ifndef S11N_SERIAL_H
#define S11N_SERIAL_H

#include <string>
#include "impl/str_view.h"
//#include "../s11n/impl/name_of_type_from_pf.h"

namespace s11n
{

template <typename T>
struct Serial;

//===================================================================================

////template <typename T>
//template <>
//struct Serial< typename std::enable_if<std::is_arithmetic<T>::value,T>::type >
//{
//    // static const std::string signature() -- only if can
//};

} // namespace s11n


#endif // S11N_SERIAL_H
