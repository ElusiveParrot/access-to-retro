#include <utility>
#include "error/io-error.hpp"

ar::error::io_error::io_error(std::string source, std::string error) :
        ar::base_exception("global"),
        _source(std::move(source)),
        _error(std::move(error))
{

}

std::string ar::error::io_error::get_source() const
{
    return _source;
}

std::string ar::error::io_error::get_error() const
{
    return _error;
}

std::string ar::error::io_error::get_logger_formatted_error() const
{
    return "IO error caused by file/source: '" + get_source() + "' with message: " + get_error();
}
