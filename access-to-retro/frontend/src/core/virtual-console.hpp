/**
 * @file core/virtual-console.hpp
 */

#ifndef ACCESS_TO_RETRO_FRONTEND_VIRTUAL_CONSOLE_HPP
#define ACCESS_TO_RETRO_FRONTEND_VIRTUAL_CONSOLE_HPP

#include <functional>
#include <thread>
#include <string>
#include "helpers/types.hpp"
#include "executable-binary.hpp"
#include "dynamic-library.hpp"

namespace ar::core
{
    /**
     * @brief Each emulator is called a 'virtual console' in this frontend, this class represents a single emulator.
     * @details It's a 'wrapper' class around raw dynamic library with extra functionality so that the emulator can
     *          actually work.
     * @warning Meta: Each virtual console should be treated as a full standalone program in terms of security, it is
     *          possible for an attacker to run pretty much any code inside the virtual console so any virtual
     *          console should be audited by the user as any other program downloaded from the internet would.
     */
    class virtual_console
    {
    public:
        /**
         * @brief Default constructor
         * @param path Path to the '.vc' file of a virtual console
         * @throws Exceptions:
         *  - ar::error::io_error: Invalid .vc file path
         *  - ar::core::virtual_console_error: Unable to create virtual console
         */
        explicit virtual_console(const std::string& path);

        /**
         * @brief Move constructor
         * @param other Source rvalue
         * @warning All threads are set to nullptr and atomic thread running bool is set to false
         */
        virtual_console(virtual_console&& other) noexcept;

        ~virtual_console();

        /**
         * @brief Prepares the virtual console for starting
         * @param binary Binary file with executable content to be linked
         * @details Runs '_ar_vc_startup' function allocating virtual console and library resources
         * @warning Binary file is stored as reference, ownership remains with main window class
         * @warning If this function runs you must use 'quit_and_cleanup' later otherwise memory leak can happen
         * @throws Exceptions:
         *  - ar::core::virtual_console_error: 'ar_vc_startup' returned non-zero error code
         */
        void prepare_for_startup(std::unique_ptr<ar::core::executable_binary>& binary);

        /// @brief Create developer's defined main, render and input threads and start them
        void create_and_run_threads();

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
        T get_symbol_from_library(const std::string& symbol_name)
        {
            // Pass through to ar::core::dynamic_library 's get_symbol function
            return _library.get_symbol<T>(symbol_name);
        }

        /**
         * @brief Cleans up all the resources that were allocated by the virtual console
         * @details This function should only get called when the virtual console closes after it was started, that's
         *          why it's a separate method and not a destructor as a destructor would run even if virtual console
         *          was not started yet.
         */
        void quit_and_cleanup();

        /**
         * @brief Get the path from which the virtual console was created
         * @return Path of the library file used to create virtual console
         */
        [[nodiscard]] std::string get_path() const;

        /**
         * @brief Get the name of the virtual console as defined by the developer
         * @return Name of the virtual console
         */
        [[nodiscard]] std::string get_name() const;

        /**
         * @brief Get the system of the virtual console as defined by the developer
         * @return Name system emulated by the virtual console
         */
        [[nodiscard]] std::string get_system() const;

        /**
         * @brief Get the name/surname/both of the virtual console author as defined by the developer
         * @return Author of the virtual console developer
         */
        [[nodiscard]] std::string get_author() const;

        /**
         * @brief Get the game extensions of the virtual console as defined by the developer
         * @return Extensions of games that this virtual console support
         */
        [[nodiscard]] std::string get_rom_extension() const;

        /**
         * @brief Get the default window width as defined by the developer
         * @return Window width recommended by the virtual console
         */
        [[nodiscard]] unsigned get_default_window_width() const;

        /**
         * @brief Get the default window height as defined by the developer
         * @return Window height recommended by the virtual console
         */
        [[nodiscard]] unsigned get_default_window_height() const;

    private:
        /// @brief Path from which the virtual console was loaded
        std::string _path;

        /// @brief The dynamic library loaded for this virtual console
        ar::core::dynamic_library _library;

        /// @brief Name of the virtual console (NOT the emulated system)
        std::string _name;

        /// @brief Name of the emulated system, please see guide for standard names for common consoles
        std::string _system;

        // @brief Name/nickname of this virtual console's author
        std::string _author;

        /// @brief File extension associated with this system for ex: ".gb" for GameBoy
        std::string _rom_extension;

        /// @brief Frame time for the virtual console, it is calculated using formula: 1000 / Frame Rate
        double _frame_time;

        /// @brief Default window width for this virtual console
        unsigned _default_window_width;

        /// @brief Default window height for this virtual console
        unsigned _default_window_height;

        /// @brief Binary linked to this virtual console, linked using 'prepare_for_startup'
        std::optional<std::reference_wrapper<std::unique_ptr<ar::core::executable_binary> > > _linked_binary;

        // TODO ar_version

        /// @brief This function gets called when virtual console is loaded (vc object created)
        std::function<ar::types::err_code()> _define_fn;

        /// @brief This function gets called on virtual console startup
        std::function<ar::types::err_code()> _startup_fn;

        /// @brief This function gets called when virtual console exits
        std::function<void()> _quit_fn;

        /// @brief Thread-safe boolean to check if threads should still be running
        std::atomic_bool _run_threads = false;

        /// @brief Main thread object
        std::unique_ptr<std::thread> _main_thread = nullptr;

        /// @brief Function used to create 'main' thread
        std::function<void()> _main_thread_fn;

        /// @brief Render thread object
        std::unique_ptr<std::thread> _render_thread = nullptr;

        /// @brief Function used to create 'render' thread
        std::function<void()> _render_thread_fn;

        /// @brief Input thread object
        std::unique_ptr<std::thread> _input_thread = nullptr;

        /// @brief Function used to create 'input' thread
        std::function<void()> _input_thread_fn;
    };

    /// @brief Exception used by virtual_console class
    class virtual_console_error : public ar::base_exception
    {
    public:
        virtual_console_error(std::string virtual_console_name, std::string error);

        [[nodiscard]] std::string get_vc_name() const;
        [[nodiscard]] std::string get_error() const;

        [[nodiscard]] std::string get_logger_formatted_error() const override;

    private:
        const std::string _virtual_console_name;
        const std::string _error;
    };
}

#endif // ACCESS_TO_RETRO_FRONTEND_VIRTUAL_CONSOLE_HPP
