/**
 * @file emulator/emulator.hpp
 */

#ifndef ACCESS_TO_RETRO_EMULATOR_HPP
#define ACCESS_TO_RETRO_EMULATOR_HPP

#include <access-to-retro-dev/access-to-retro-dev.h>
#include <memory>
#include "frame-buffer.hpp"
#include "controller.hpp"
#include "ram-memory.hpp"
#include "gpu.hpp"
#include "cpu.hpp"

/// @brief Root namespace of the project
namespace ar::chip8
{
    /// @brief CHIP8's Frame rate (frames per second shown)
    constexpr uint32_t FRAME_RATE = 60;

    /// @brief CHIP8's Frame time (time per each frame shown in milliseconds)
    constexpr double FRAME_TIME = (1 / static_cast<double>(FRAME_RATE)) * 1000;

    /// @brief CHIP8's clock speed (600 hz)
    constexpr uint32_t CLOCK_SPEED = 600;

    /// @brief Main emulator object
    class emulator
    {
    public:
        emulator();

        /// @brief Create global emulator object
        static void create_global_emulator();

        /**
         * @brief The emulator object is global, this is a getter to it (similar to settings_managers in frontend)
         * @return Pointer to global emulator object
         */
        static std::shared_ptr<ar::chip8::emulator> get_global_emulator();

        /**
         * @brief Getter for GPU object
         * @return Emulator's GPU object
         */
        [[nodiscard]] ar::chip8::gpu& access_gpu();

        /**
         * @brief Getter for CPU object
         * @return Emulator's CPU object
         */
        [[nodiscard]] ar::chip8::cpu& access_cpu();

        /**
         * @brief Getter for controller object
         * @return Emulator's controller object
         */
        [[nodiscard]] ar::chip8::controller& access_controller();

        /**
         * @brief Getter for RAM object
         * @return Emulator's RAM object
         */
        [[nodiscard]] ar::chip8::ram_memory& access_ram();

    private:
        /// @brief Object emulating CHIP8's controller
        ar::chip8::controller _controller {};

        /// @brief Object emulating CHIP8's RAM memory
        ar::chip8::ram_memory _ram {};

        /// @brief Object emulating CHIP8's GPU
        ar::chip8::gpu _gpu;

        /// @brief Object emulating CHIP8's CPU
        ar::chip8::cpu _cpu;
    };
}

#endif //ACCESS_TO_RETRO_EMULATOR_HPP
