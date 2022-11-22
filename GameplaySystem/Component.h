#pragma once

#include "Object.h"
#include "GameObject.h"
#include "../monowrapper/monopp/mono_object.h"
#include "../monowrapper/monopp/mono_property_invoker.h"
#include "../monowrapper/monopp/mono_method_invoker.h"

namespace engine {

class Component : public Object{
    friend class GameObject;

public:
    std::string Name() const;
    std::shared_ptr<GameObject> GameObject();

    Component(const mono::mono_assembly& assembly , mono::mono_object object);

    static void CacheMethods(const mono::mono_assembly& assembly);

private:
    const mono::mono_assembly& assembly_;

    static mono::mono_property_invoker* game_object_;
};

} // namespace engine
