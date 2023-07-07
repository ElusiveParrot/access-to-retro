#include "util/logger.hpp"
#include "dynamic-library.hpp"

ar::core::dynamic_library::dynamic_library(const std::string& path)
{
    // Open dynamic library ('.dll' on windows, '.a'/'.dylib' on unix/macos)
#if defined(__linux__) || defined(__APPLE__)
    _lib_handle = dlopen(path.c_str(), RTLD_NOW);
#elif defined(_WIN32)
    _lib_handle = LoadLibraryA(path.c_str());
#endif

    if (_lib_handle == nullptr)
    {
        throw ar::error::io_error(path, "Unable to open dynamic library");
    }

    _lib_path = path;
}

ar::core::dynamic_library::~dynamic_library()
{
    if (_lib_handle != nullptr)
    {
        // Close dynamic library if it was opened
#if defined(__linux__) || defined(__APPLE__)
        int close_successful = dlclose(_lib_handle) == 0;
#elif defined(_WIN32)
        int close_successful = FreeLibrary(_lib_handle) != 0;
#endif
        if (!close_successful)
        {
            // Exception shouldn't be thrown here (it's a destructor, and it doesn't really matter if it fails to be honest)
            std::string err_msg = "Error when unloading dynamic library at: '" + _lib_path + "'. Error: "
                                  + get_error_message();
            LOG_ERROR(err_msg);
        }
    }
}

std::string ar::core::dynamic_library::get_error_message()
{
    // Query OS's API to get error
#if defined(__linux__) || defined(__APPLE__)
    const char* err = dlerror();
    return err == nullptr ? "Unknown error (id=0)" : std::string(err);
#elif defined(_WIN32)
    DWORD error = GetLastError();
    if (error == 0)
    {
        return "Unknown error (id=0)";
    }

    // Create message buffer
    LPSTR buffer = nullptr;

    // Get message and size from Windows API
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&buffer, 0,
        NULL);

    // Convert to C++ string
    std::string message(buffer, size);

    // Free message buffer
    LocalFree(buffer);

    return message;
#endif
}
