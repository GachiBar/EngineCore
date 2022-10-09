#pragma once

#include <memory>

#include "LayerStack.h"
#include "../GameplaySystem/Game.h"

typedef engine::Game Engine ;

class Application
{
public:
	
	Application();

    void PushLayer();

    /// Setup before engine initialization
    virtual void Setup() { }

    /// Setup after engine initialization and before running the main loop
    virtual void Start() { }

    /// Cleanup after the main loop. Called by Application
    virtual void Stop() { }

    /// Initialize the engine and run the main loop, then return the application exit code
    int Run();

    virtual ~Application() = default;
protected:
    LayerStack m_LayerStack;
    std::shared_ptr<Engine> engine_;

    /// Application exit code.
    int exit_code_;
};
