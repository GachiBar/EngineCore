﻿#pragma once

#include "Object.h"
#include "../Core/libs/Delegates.h"
#include "../monowrapper/monopp/mono_object.h"
#include "../monowrapper/monopp/mono_method_invoker.h"
#include "../monowrapper/monopp/mono_property_invoker.h"

#include <string>

namespace engine {

class Scene;
class Component;

class GameObject : public Object {
public:
    // TODO: Find way to sync links
    // TODO: Write getting reference type from mono_type
    // TODO: Write dem funcs
    // Add & Get can be linked to one instance btw
    std::string Name() const;
    void Name(std::string& value);
    size_t Count() const;

    DECLARE_EVENT(ComponentAddedEvent, GameObject, GameObject&, std::shared_ptr<Component>)
    ComponentAddedEvent ComponentAdded;

    DECLARE_EVENT(ComponentRemovedEvent, GameObject, GameObject&, std::shared_ptr<Component>)
    ComponentRemovedEvent ComponentRemoved;

    GameObject(const mono::mono_assembly& assembly, mono::mono_object object);

    std::shared_ptr<Component> AddComponent(const TypeData& type_data);
    std::shared_ptr<Component> AddComponent(const std::string& name_space, const std::string& name);
    std::shared_ptr<Component> AddComponent(const mono::mono_type& component_type);
    void RemoveComponent(std::shared_ptr<Component> component);
    std::shared_ptr<Component> GetComponent(const TypeData& type_data);
    std::shared_ptr<Component> GetComponent(const std::string& name_space, const std::string& name);
    std::shared_ptr<Component> GetComponent(const mono::mono_type& component_type);

    // Cached methods
    void Initialize();
    void FixedUpdate();
    void Update();
    void Render();
    void Terminate();
    void Invalidate();

    std::shared_ptr<Component> operator[](size_t index) const;

    static void CacheMethods(const mono::mono_assembly& assembly);

private:
    const mono::mono_assembly& assembly_;

    static mono::mono_property_invoker* name_;
    static mono::mono_property_invoker* count_;
    static mono::mono_method_invoker* get_item_;

    static mono::mono_method_invoker* add_component_;
    static mono::mono_method_invoker* remove_component_;
    static mono::mono_method_invoker* get_component_;

    static mono::mono_method_invoker* initialize_;
    static mono::mono_method_invoker* fixed_update_;
    static mono::mono_method_invoker* update_;
    static mono::mono_method_invoker* render_;
    static mono::mono_method_invoker* terminate_;

    static mono::mono_method_invoker* invalidate_;    
};

} // namespace engine


