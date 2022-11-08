#pragma once

#include <memory>

#include "LayerStack.h"
#include "../GameplaySystem/Engine.h"
#include "../monowrapper/monopp/mono_jit_domain.h"
#include "../monowrapper/monopp/mono_domain.h"
#include "../monowrapper/monopp/mono_assembly.h"
#include "../monowrapper/monopp/mono_type.h"
#include "../monowrapper/monopp/mono_object.h"

#include "../Core/Interfaces/IWindowsApplication.h"


class Application : public IWindowsApplication
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

    std::shared_ptr<FGenericWindow> GetMainWindow() override;

    engine::Engine* GetEngine() const override;
    void ResizeBackBuffer(int32 InWidth, int32 InHeight) override;
    void ResizeViewport(int32 InWidth, int32 InHeight) override;
    

    const mono::mono_assembly& GetAssembly() const;
    virtual WNDPROC GetWndProc() override;

    virtual ~Application() = default;

    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
protected:

    bool is_exit_requested = false;

    void ApplyInput();
public:

protected:
	LayerStack m_LayerStack;
    std::vector<std::shared_ptr<FGenericWindow>> wnds;

    mono::mono_jit_domain m_JitDomain;
    mono::mono_domain m_Domain;
    mono::mono_assembly m_Assembly;

    std::shared_ptr<engine::Engine> engine_;


    /// Application exit code.
    int exit_code_;

    
private:
    static const char* kMonoLibPath;
    static const char* kDllPath;
};
