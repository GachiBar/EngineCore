#pragma once

#include <memory>

#include "LayerStack.h"
#include "../GameplaySystem/Engine.h"
#include "../monowrapper/monopp/mono_domain.h"
#include "../monowrapper/monopp/mono_assembly.h"
#include "../monowrapper/monopp/mono_type.h"
#include "../monowrapper/monopp/mono_object.h"
#include "../monowrapper/monopp/mono_method.h"
#include "../monowrapper/monopp/mono_method_invoker.h"

class Application
{
public:
	Application(const char* dll_path);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);
    
    /// OnSetup before engine initialization
    virtual void OnSetup();

    /// OnSetup after engine initialization and before running the main loop
    virtual void OnStart();


    /// Cleanup after the main loop. Called by Application
    virtual void OnStop() { }

    /// Initialize the engine and run the main loop, then return the application exit code
    int Run();

    void Close();
    mono::mono_object CreateGameObject(const mono::mono_object& scene);

    mono::mono_object AddComponent(
        const mono::mono_assembly& assembly, 
        const mono::mono_object& go, 
        const std::string& name_space, 
        const std::string& name);

    engine::Engine* GetEngine() const;

    virtual ~Application() = default;
protected:

    bool is_exit_requested = false;

    void ApplyInput();

    LayerStack m_LayerStack;
    mono::mono_domain m_Domain;
    mono::mono_assembly m_Assembly;
    std::shared_ptr<engine::Engine> engine_;

    /// Application exit code.
    int exit_code_;

private:

    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    void RegisterWindowClass(HINSTANCE instance, LPCWSTR window_name);
    HWND CreateWindowInstance(HINSTANCE instance, LPCWSTR window_name, LONG width, LONG height);
};
