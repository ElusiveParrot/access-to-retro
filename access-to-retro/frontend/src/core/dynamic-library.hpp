/**
 * @file core/dynamic-library.hpp
 */

#ifndef ACCESS_TO_RETRO_FRONTEND_DYNAMIC_LIBRARY_HPP
#define ACCESS_TO_RETRO_FRONTEND_DYNAMIC_LIBRARY_HPP

#if defined(__linux__) || defined(__APPLE__)

#include <dlfcn.h>

#elif defined(_WIN32)

#include <windows.h>
#include <libloaderapi.h>
#include <errhandlingapi.h>
#include <winnt.rh>

#endif

#include <functional>
#include <stdexcept>
#include <string>
#include "error/io-error.hpp"
#include "error/os-error.hpp"
#include "helpers/types.hpp"

/// @brief Contains classes and function that compromise the 'backend' of the application for ex: emulators themselves.
namespace ar::core
{
    /// @brief Represents a dynamically linked library file (for example: '.dll' on windows).
    class dynamic_library
    {
    public:
        /**
         * @brief Default constructor
         * @throws Exceptions:
         *  - ar::error::io_error: When the library file could not be opened
         * @param path Path of the library
         */
        explicit dynamic_library(const std::string& path);
        ~dynamic_library();

        /**
         * @brief Fetches the requested symbol's address by name and casts it to T
         * @throws Exceptions:
         *  - std::runtime_error: When the library has not been loaded
         *  - ar::core::os_error: When the symbol was not found inside the library
         * @tparam T Type that the result will be casted to.
         * @param name Name of the symbol as defined in the library.
         * @return Symbol's address casted to T
         */
        template<class T>
        T get_symbol(const std::string& name)
        {
            if (_lib_handle == nullptr || _lib_path.empty())
            {
                throw std::runtime_error("Attempting to find symbol '" +
                                         name + "' from dynamic library that was not loaded");
            }

            //  Query the dynamic library for a specific symbol (most likely a function)
#if defined(__linux__) || defined(__APPLE__)
            void* fn = dlsym(_lib_handle, name.c_str());
#elif defined(_WIN32)
            void* fn = (void*) GetProcAddress(_lib_handle, name.c_str());
#endif
            if (fn == nullptr)
            {
                throw ar::error::os_error("Unable to find symbol '" + name + "' in dynamic library at: " + _lib_path +
                                          ". Error: " + get_error_message());
            }

            // Cast the symbol from generic memory to provided type, use C style casting as type unknown
            return (T) fn;
        }

    private:
        std::string _lib_path;

#if defined(__linux__) || defined(__APPLE__)
        /// @brief 'Handle' of the dynamic library, a pointer to a generic memory where the library was loaded
        ar::types::c_generic_memory_ptr _lib_handle;
#elif defined(_WIN32)
        HMODULE _lib_handle;
#endif

        /**
         * @brief Query OS's API to get error message for last dynamic library loading error
         * @return Error string
         */
        static std::string get_error_message();
    };
}


#endif //ACCESS_TO_RETRO_FRONTEND_DYNAMIC_LIBRARY_HPP
