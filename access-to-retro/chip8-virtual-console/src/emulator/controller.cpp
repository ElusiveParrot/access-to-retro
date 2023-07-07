#include "controller.hpp"

void ar::chip8::controller::set_key_status(ar::chip8::key key, ar_unified_controller_key_status status)
{
    _key_pad[static_cast<size_t>(key)] = status == ar_key_status_pressed;
}

bool ar::chip8::controller::is_key_pressed(ar::chip8::key key)
{
    return _key_pad[static_cast<size_t>(key)];
}
