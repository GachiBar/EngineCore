#pragma once

#include <memory>

#include "LayerStack.h"
#include "../GameplaySystem/Engine.h"
#include "../monowrapper/monopp/mono_jit_domain.h"
#include "../monowrapper/monopp/mono_domain.h"
#include "../monowrapper/monopp/mono_assembly.h"
#include "../monowrapper/monopp/mono_type.h"
#include "../monowrapper/monopp/mono_object.h"
#include "../monowrapper/monopp/mono_method.h"
#include "../monowrapper/monopp/mono_method_invoker.h"
#include "InputSystem/UnrealCoreSystem/InputSettings.h"
#include "InputSystem/UnrealCoreSystem/Keys.h"
#include "InputSystem/UnrealCoreSystem/Windows/GenericWindow.h"

class Application
{
public:
	Application();

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

    std::shared_ptr<FGenericWindow> GetMainWindow();

    mono::mono_object CreateGameObject(const mono::mono_object& scene);

    mono::mono_object AddComponent(
        const mono::mono_assembly& assembly, 
        const mono::mono_object& go, 
        const std::string& name_space, 
        const std::string& name);

    engine::Engine* GetEngine() const;

    virtual ~Application() = default;

    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
protected:

    bool is_exit_requested = false;

    void ApplyInput();

    LayerStack m_LayerStack;
    std::vector<std::shared_ptr<FGenericWindow>> wnds;

    mono::mono_jit_domain m_JitDomain;
    mono::mono_domain m_Domain;
    mono::mono_assembly m_Assembly;
    std::shared_ptr<engine::Engine> engine_;
    std::shared_ptr<InputSettings> input_settings;

    /// Application exit code.
    int exit_code_;

    
private:
    static const char* kMonoLibPath;
    static const char* kDllPath;

   
    void RegisterWindowClass(HINSTANCE instance, LPCWSTR window_name);
    HWND CreateWindowInstance(HINSTANCE instance, LPCWSTR window_name, LONG width, LONG height);
};
