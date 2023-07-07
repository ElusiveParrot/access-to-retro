#include "base-exception.hpp"

ar::base_exception::base_exception(std::string module) :
        std::runtime_error("[" + module + "]: Unknown error, base exception class thrown."),
        _module(std::move(module))
{

}

std::string ar::base_exception::get_module() const
{
    return _module;
}
