#pragma once

#include "Object.h"
#include "Runtime.h"
#include "../Core/libs/Delegates.h"

#include <string>

namespace engine {

class Scene;
class Component;

class GameObject : public Object {
public:
    std::string Name() const;
    void Name(std::string& value);
    size_t Count() const;

    DECLARE_EVENT(ComponentAddedEvent, GameObject, GameObject&, std::shared_ptr<Component>)
    ComponentAddedEvent ComponentAdded;

    DECLARE_EVENT(ComponentRemovedEvent, GameObject, GameObject&, std::shared_ptr<Component>)
    ComponentRemovedEvent ComponentRemoved;

    GameObject(const Runtime& runtime, mono::mono_object object);

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

    static void CacheMethods(const Runtime& runtime);

private:
    const Runtime& runtime_;

    static Property* name_;
    static Property* count_;
    static Method* get_item_;

    static Method* add_component_;
    static Method* remove_component_;
    static Method* get_component_;

    static Method* initialize_;
    static Method* fixed_update_;
    static Method* update_;
    static Method* render_;
    static Method* terminate_;

    static Method* invalidate_;    
};

} // namespace engine


