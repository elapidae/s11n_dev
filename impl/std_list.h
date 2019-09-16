#ifndef STD_LIST_H
#define STD_LIST_H

#include <list>

//=======================================================================================
//  У списка в 11-ом G++ название плохо выводится, проще самим специфицировать.
//vdeb << signature< list<bool> >(); // std::__cxx11::list<bool>
namespace s11n
{
    template<typename T> struct Serial;

    template<typename T>
    struct Serial<std::list<T>>
    {
        static constexpr auto name_of_type = "std::list";
    };
}
//=======================================================================================

#endif // STD_LIST_H
