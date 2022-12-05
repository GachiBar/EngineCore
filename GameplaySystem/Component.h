#pragma once

#include "Object.h"
#include "Runtime.h"

namespace engine {

class GameObject;

class Component : public Object{
public:
    std::string Name() const;
    std::shared_ptr<GameObject> GameObject();

    Component(const Runtime& runtime , mono::mono_object object);

    static void CacheMethods(const Runtime& runtime);

private:
    const Runtime& runtime_;

    static Property* game_object_;
};

} // namespace engine
