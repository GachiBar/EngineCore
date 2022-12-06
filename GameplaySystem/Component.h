#pragma once

#include "Object.h"
#include "Runtime.h"

namespace engine {

class GameObject;

class Component : public Object{
public:
    using Object::Object;

    std::string Name() const;
    std::shared_ptr<GameObject> GameObject(); 

    Component(const Object& other);
    Component(Object&& other) noexcept;

    static void CacheMethods(const Runtime& runtime);

private:
    static Property* game_object_;
};

} // namespace engine
