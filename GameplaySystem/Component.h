#pragma once

#include "Object.h"
#include "Runtime.h"

namespace engine {

class GameObject;

class Component : public Object{
public:
    std::string Name() const;
    std::shared_ptr<GameObject> GameObject(); 

    Component(MonoObject* mono_object);
    Component(const mono::mono_object& object);
    Component(const Object& other);
    Component(Object&& other) noexcept;

    void Invalidate(const std::string& fieldName);

    static void CacheMethods(const Runtime& runtime);

private:
    static Property* game_object_;

    Method invalidate_;

    Method GetInvalidateMethod();
};

} // namespace engine
