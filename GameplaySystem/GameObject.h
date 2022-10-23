#pragma once
#include <string>

#include "../monowrapper/monopp/mono_object.h"
#include "../monowrapper/monopp/mono_method_invoker.h"

#include "Component.h"

class Scene;
class Component;

class GameObject {
    friend class Scene;

public:
    // TODO: Find way to sync links
    // TODO: Write getting reference type from mono_type
    // TODO: Write dem funcs
    // Add & Get can be linked to one instance btw
    const mono::mono_object& GetInternal();
    
    std::shared_ptr<Component> AddComponent(const std::string& name_space, const std::string& name);
    std::shared_ptr<Component> AddComponent(const mono::mono_type& component_type);    
    void RemoveComponent(std::shared_ptr<Component> component);
    std::shared_ptr<Component> GetComponent(const std::string& name_space, const std::string& name);
    std::shared_ptr<Component> GetComponent(const mono::mono_type& component_type);   
    
    // Cached methods
    void Initialize();
    void FixedUpdate();
    void Update();
    void Render();
    void Terminate();
    void Invalidate();

    static void CacheMethods(const mono::mono_assembly& assembly);

private:
    const mono::mono_assembly& assembly_;
    mono::mono_object object_;
    
    static mono::mono_method_invoker* add_component;
    static mono::mono_method_invoker* remove_component;
    static mono::mono_method_invoker* get_component;
    
    static mono::mono_method_invoker* initialize_;
    static mono::mono_method_invoker* fixed_update_;
    static mono::mono_method_invoker* update_;
    static mono::mono_method_invoker* render_;
    static mono::mono_method_invoker* terminate_;
    static mono::mono_method_invoker* invalidate_;

    GameObject(const mono::mono_assembly& assembly, mono::mono_object object);
};
