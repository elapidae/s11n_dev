#include "s11n_writer.h"

using namespace s11n;



void Writer::write_str(const std::string &str)
{
    _buffer += str;
}

const std::string &Writer::result() const
{
    return _buffer;
}
