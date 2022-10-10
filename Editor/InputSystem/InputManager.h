#pragma once
#include "Keyboard.h"
#include "Mouse.h"

class InputManager
{
public:
    static InputManager& getInstance();

    Keyboard& GetKeyboardDevice();

    Mouse& GetMouseDevice();

private:
    InputManager();

    Keyboard kbd;
    Mouse mouse;
public:
    InputManager(InputManager const&) = delete;
    void operator=(InputManager const&) = delete;

};
