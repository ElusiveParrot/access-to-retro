/**
 * @file error/io-error.hpp
 */

#ifndef ACCESS_TO_RETRO_FRONTEND_IO_HPP
#define ACCESS_TO_RETRO_FRONTEND_IO_HPP

#include <stdexcept>
#include <string>
#include "base-exception.hpp"

namespace ar::error
{
    /**
     * @brief Input/output operation exception.
     * @details Should be the default exception for any I/O operation such as reading files, writing to files etc.
     */
    class io_error : public ar::base_exception
    {
    public:
        io_error(std::string source, std::string error);

        [[nodiscard]] std::string get_source() const;
        [[nodiscard]] std::string get_error() const;

        [[nodiscard]] std::string get_logger_formatted_error() const override;

    private:
        const std::string _source;
        const std::string _error;
    };
} // util

#endif //ACCESS_TO_RETRO_FRONTEND_IO_HPP
