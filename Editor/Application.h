#pragma once

#include <memory>


#include "LayerStack.h"
#include "../GameplaySystem/Engine.h"

class ImGuiLayer;

class Application
{
public:
	
	Application();

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);
    
    /// OnSetup before engine initialization
    virtual void OnSetup() { }

    /// OnSetup after engine initialization and before running the main loop
    virtual void OnStart();


    /// Cleanup after the main loop. Called by Application
    virtual void OnStop() { }

    /// Initialize the engine and run the main loop, then return the application exit code
    int Run();

    void Close();

    engine::Engine* GetEngine() const;

    virtual ~Application() = default;
protected:

    bool is_exit_requested = false;

    void ApplyInput();

    LayerStack m_LayerStack;
    std::shared_ptr<engine::Engine> engine_;
    ImGuiLayer* m_ImGuiLayer;

    /// Application exit code.
    int exit_code_;

private:

    static bool IsCapturedEvent(UINT uMsg);
   

    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    void RegisterWindowClass(HINSTANCE instance, LPCWSTR window_name);
    HWND CreateWindowInstance(HINSTANCE instance, LPCWSTR window_name, LONG width, LONG height);
};
