#pragma once
#define NOMINMAX

#include "Windows.h"
#include "Mouse.h"
#include "InputCoreSystem/NameMapping/PlayerInput.h"
#include "InputCoreSystem/MessageHandler/WindowsApplicationMessageHandler.h"


class IApplication;

class InputManager
{
public:
    static InputManager& getInstance();

    Mouse& GetMouseDevice();

    static bool IsInputMessage(UINT msg);

    static bool IsMouseMessage(UINT msg);

    static bool IsMouseButton(UINT msg);

    static bool IsKeyboardMessage(UINT msg);

    void ProcessInput(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    void Flush();

    std::pair<float, float> GetMousePosition() const;

    bool ReadEvent(std::shared_ptr<FInputEvent>& IE) const;

    PlayerInput* GetPlayerInput() const;
    void SetPlayerInput(PlayerInput* InPlayerEnums);

	FGenericApplicationMessageHandler* GetMessageHandler();

    IApplication* app;

    std::shared_ptr<InputSettings> input_settings;
    std::shared_ptr<PlayerInput> player_input;

private:

    InputManager();

    Mouse mouse;
public:
    InputManager(InputManager const&) = delete;
    void operator=(InputManager const&) = delete;

};


