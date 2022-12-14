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
#include "../Logger/LogManager.h"

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
    virtual void OnStop();


    /// Initialize the engine and run the main loop, then return the application exit code
    int Run(int argc, char* argv[]);
    void DrawGameUI();

    void Close();

    std::shared_ptr<FGenericWindow> GetMainWindow() override;

    engine::Engine* GetEngine() const override;
    void ResizeBackBuffer(int32 InWidth, int32 InHeight) override;
    void ResizeViewport(int32 InWidth, int32 InHeight) override;

    ILayer* GetCurrentUpdateLayer() override;
    EEditorInputMode::Type GetCurrentInputMode() const override;

    const engine::Runtime& GetRuntime() const;
    virtual WNDPROC GetWndProc() override;

    virtual ~Application() = default;

    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
protected:

    bool is_exit_requested = false;

    void ApplyInput();
public:

protected:
    Layer* current_layer_on_update;

	LayerStack m_LayerStack;
    std::vector<std::shared_ptr<FGenericWindow>> wnds;

    engine::Runtime runtime;

    std::shared_ptr<engine::Engine> engine_;


    /// Application exit code.
    int exit_code_;
private:
    static const char* kMonoLibPath;
    static const char* kDllPath;
};
