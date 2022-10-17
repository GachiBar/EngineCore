#pragma once
#define NOMINMAX

#include "InputEvent.h"
#include "Windows.h"
#include "Keyboard.h"
#include "Mouse.h"

class InputManager
{
public:
    static InputManager& getInstance();

    Keyboard& GetKeyboardDevice();

    Mouse& GetMouseDevice();

    static bool IsInputMessage(UINT msg);

    static bool IsMouseMessage(UINT msg);

    static bool IsKeyboardMessage(UINT msg);

    void ProcessInput(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    void SendEventToBuffer(std::shared_ptr<InputEvent> const & input_event);

	void SetInputBufferSize(int new_buffer_size);

    std::shared_ptr<InputEvent> ReadEvent();

    void Flush();


    std::pair<float, float> GetMousePosition() const;

    float GetWheelDelta() const;

    //Returns true while the user holds down the key
    bool GetKey(KeyboardKey key) const;

    //Returns whether the given mouse button is held down
    bool GetMouseKey(MouseKey key) const;

    //Returns true during the frame the user starts pressing down the key
    bool GetKeyDown(KeyboardKey key) const;

    //Returns true during the frame the user starts pressing up the key
    bool GetKeyUp(KeyboardKey key) const;

    //Returns true during the frame the user starts pressing down the key
    bool GetMouseKeyDown(MouseKey key) const;

    //Returns true during the frame the user starts pressing up the key
    bool GetMouseKeyUp(MouseKey key) const;


private:
    template<typename T>
    void TrimBuffer(std::queue<T>& buffer);

private:
    int bufferSize;
    std::queue<std::shared_ptr<InputEvent>> input_buffer;

    InputManager();

    Keyboard kbd;
    Mouse mouse;
public:
    InputManager(InputManager const&) = delete;
    void operator=(InputManager const&) = delete;

};


