#pragma once
#define NOMINMAX

#include "Windows.h"
#include "Mouse.h"
#include "UnrealCoreSystem/NameMapping/PlayerInput.h"
#include "UnrealCoreSystem/MessageHandler/WindowsApplicationMessageHandler.h"

class Application;

class InputManager
{
public:
    static InputManager& getInstance();

    Mouse& GetMouseDevice();

    static bool IsInputMessage(UINT msg);

    static bool IsMouseMessage(UINT msg);

    static bool IsKeyboardMessage(UINT msg);

    void ProcessInput(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    void Flush();

    std::pair<float, float> GetMousePosition() const;

    bool ReadEvent(FInputEvent& IE) const;

    Application* app;
private:

private:
    PlayerInput* player_input;

    FGenericApplicationMessageHandler* message_handler;

    InputManager();

    Mouse mouse;
public:
    InputManager(InputManager const&) = delete;
    void operator=(InputManager const&) = delete;

};


