/**
 * @file error/os-error.hpp
 */

#ifndef ACCESS_TO_RETRO_FRONTEND_OS_ERROR_HPP
#define ACCESS_TO_RETRO_FRONTEND_OS_ERROR_HPP

#include <stdexcept>
#include <string>
#include "base-exception.hpp"

/// @brief Contains custom global generic exception classes
namespace ar::error
{
    /**
     * @brief Exception related to actions on OS level.
     * @details Should be used for OS specific actions for ex: dynamic library loading.
     */
    class os_error : public ar::base_exception
    {
    public:
        os_error(std::string error);

        [[nodiscard]] std::string get_error() const;

        [[nodiscard]] std::string get_logger_formatted_error() const override;

    private:
        const std::string _error;
    };
}

#endif //ACCESS_TO_RETRO_FRONTEND_OS_ERROR_HPP
