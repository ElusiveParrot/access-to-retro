#include <utility>
#include "helpers/cross-platform-helper.hpp"
#include "os-error.hpp"

ar::error::os_error::os_error(std::string error) :
        ar::base_exception("global"),
        _error(std::move(error))
{

}

std::string ar::error::os_error::get_error() const
{
    return _error;
}

std::string ar::error::os_error::get_logger_formatted_error() const
{
    return "(" + ar::helpers::cross_platform::get_os_name() + "): " + get_error();
}
