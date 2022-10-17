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

    /// Setup before engine initialization
    virtual void Setup() { }

    /// Setup after engine initialization and before running the main loop
    virtual void Start() { }

    /// Cleanup after the main loop. Called by Application
    virtual void Stop() { }

    /// Initialize the engine and run the main loop, then return the application exit code
    int Run();

    mono::mono_object CreateGameObject(const mono::mono_object& scene) {
        mono::mono_method create_go_method(scene.get_type(), "CreateGameObject", 0);
        mono::mono_method_invoker create_go_method_invoker(create_go_method);
        mono::mono_object go(create_go_method_invoker.invoke(scene));
        
        mono::mono_method invalidate_scene_method(scene.get_type(), "Invalidate", 0);
        mono::mono_method_invoker invalidate_scene_method_invoker(invalidate_scene_method);
        invalidate_scene_method_invoker.invoke(scene);

        return go;
    }

    mono::mono_object AddComponent(
        const mono::mono_assembly& assembly, 
        const mono::mono_object& go, 
        const std::string& name_space, 
        const std::string& name) 
    {       
        mono::mono_method add_component_method(go.get_type(), "AddComponent", 1);
        mono::mono_method_invoker add_component_method_invoker(add_component_method);

        MonoDomain* domain = mono::mono_domain::get_current_domain().get_internal_ptr();
        
        mono::mono_type component_type = assembly.get_type(name_space, name);
        MonoType* type = mono_class_get_type(component_type.get_internal_ptr());
        MonoReflectionType* reflection_type = mono_type_get_object(domain,type);

        void* params[1];
        params[0] = reflection_type;

        mono::mono_object component(add_component_method_invoker.invoke(go, params));

        mono::mono_method invalidate_go_method(go.get_type(), "Invalidate", 0);
        mono::mono_method_invoker invalidate_go_method_invoker(invalidate_go_method);
        invalidate_go_method_invoker.invoke(go);
        
        return component;
    }

    std::shared_ptr<engine::Engine> GetEngine();

    virtual ~Application() = default;
protected:
    void ApplyInput();

    LayerStack m_LayerStack;
    mono::mono_domain m_Domain;
    mono::mono_assembly m_Assembly;
    std::shared_ptr<engine::Engine> engine_;

    /// Application exit code.
    int exit_code_;
};
