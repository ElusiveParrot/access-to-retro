/**
 * @file error/base-exception.hpp
 */

#ifndef ACCESS_TO_RETRO_FRONTEND_BASE_EXCEPTION_HPP
#define ACCESS_TO_RETRO_FRONTEND_BASE_EXCEPTION_HPP

#include <stdexcept>
#include <string>

namespace ar
{
    /**
     * @brief Represents the base class for all access retro exceptions in the program.
     * @details Should be the default exception base class, STD library exceptions should only be used for
     *          non access retro specific code (for example utility classes/functions)
     */
    class base_exception : public std::runtime_error
    {
    public:
        explicit base_exception(std::string module);

        [[nodiscard]] std::string get_module() const;

        /**
         * @brief Creates a logger-friendly error message. Logger should use it by itself, there should be no need to
         *        call it directly.
         * @return formatted error message
         */
        [[nodiscard]] virtual std::string get_logger_formatted_error() const = 0;

    private:
        const std::string _module;
    };
} // ar

#endif //ACCESS_TO_RETRO_FRONTEND_BASE_EXCEPTION_HPP
