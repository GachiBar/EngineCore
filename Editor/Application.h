#pragma once

#include <memory>

#include "LayerStack.h"
#include "../GameplaySystem/Engine.h"

class Application
{
public:
	
	Application();

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);


    /// Setup before engine initialization
    virtual void Setup() { }

    /// Setup after engine initialization and before running the main loop
    virtual void Start() { }

    /// Cleanup after the main loop. Called by Application
    virtual void Stop() { }

    /// Initialize the engine and run the main loop, then return the application exit code
    int Run();

    std::shared_ptr<engine::Engine> GetEngine();

    virtual ~Application() = default;
protected:
    void ApplyInput();

    LayerStack m_LayerStack;
    std::shared_ptr<engine::Engine> engine_;

    /// Application exit code.
    int exit_code_;

private:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    void RegisterWindowClass(HINSTANCE instance, LPCWSTR window_name);
    HWND CreateWindowInstance(HINSTANCE instance, LPCWSTR window_name, LONG width, LONG height);
};
