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
    using Object::Object;

    std::string Name() const;
    void Name(std::string& value);
    size_t Count() const;

    DECLARE_EVENT(ComponentAddedEvent, GameObject, GameObject&, std::shared_ptr<Component>)
    ComponentAddedEvent ComponentAdded;

    DECLARE_EVENT(ComponentRemovedEvent, GameObject, GameObject&, std::shared_ptr<Component>)
    ComponentRemovedEvent ComponentRemoved;

    GameObject(const Object& other);
    GameObject(Object&& other);

    std::shared_ptr<Component> AddComponent(const TypeData& type_data);
    std::shared_ptr<Component> AddComponent(const std::string& name_space, const std::string& name);
    std::shared_ptr<Component> AddComponent(const std::string& full_name);
    std::shared_ptr<Component> AddComponent(const Type& type);
    void RemoveComponent(std::shared_ptr<Component> component);
    std::shared_ptr<Component> GetComponent(const TypeData& type_data);
    std::shared_ptr<Component> GetComponent(const std::string& name_space, const std::string& name);
    std::shared_ptr<Component> GetComponent(const std::string& full_name);
    std::shared_ptr<Component> GetComponent(const Type& type);

    // Cached methods
    void Initialize();
    void FixedUpdate();
    void Update();
    void Render();
    void DebugRender();
    void Terminate();
    void Invalidate();

    std::shared_ptr<Component> operator[](size_t index) const;

    static void CacheMethods(const Runtime& runtime);

private:
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
    static Method* debug_render_;
    static Method* terminate_;

    static Method* invalidate_;    
};

} // namespace engine


