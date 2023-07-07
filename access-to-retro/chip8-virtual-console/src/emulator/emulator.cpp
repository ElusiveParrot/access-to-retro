#include <memory>
#include "emulator.hpp"

static std::shared_ptr<ar::chip8::emulator> g_emulator;

ar::chip8::emulator::emulator() :
        _gpu(_ram),
        _cpu(_ram, _gpu, _controller)
{

}

void ar::chip8::emulator::create_global_emulator()
{
    g_emulator = std::make_shared<ar::chip8::emulator>();
}

std::shared_ptr<ar::chip8::emulator> ar::chip8::emulator::get_global_emulator()
{
    return g_emulator;
}

ar::chip8::gpu& ar::chip8::emulator::access_gpu()
{
    return _gpu;
}

ar::chip8::cpu& ar::chip8::emulator::access_cpu()
{
    return _cpu;
}

ar::chip8::ram_memory& ar::chip8::emulator::access_ram()
{
    return _ram;
}

ar::chip8::controller& ar::chip8::emulator::access_controller()
{
    return _controller;
}
