#pragma once
#define NOMINMAX
#include "Windows.h"
#include "Keyboard.h"
#include "Mouse.h"

class InputManager
{
public:
    static InputManager& getInstance();

    Keyboard& GetKeyboardDevice();

    Mouse& GetMouseDevice();

    bool IsInputMessage(UINT msg) const;

    void ProcessInput(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
private:
    InputManager();

    Keyboard kbd;
    Mouse mouse;
public:
    InputManager(InputManager const&) = delete;
    void operator=(InputManager const&) = delete;

};
