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

    void Initialize();
    void FixedUpdate();
    void Update();
    void Render();
    void Terminate();

    static void CacheMethods(const mono::mono_assembly& assembly);

private:
    const mono::mono_assembly& assembly_;

    static mono::mono_property_invoker* game_object_;

    static mono::mono_method_invoker* initialize_;
    static mono::mono_method_invoker* fixed_update_;
    static mono::mono_method_invoker* update_;
    static mono::mono_method_invoker* render_;
    static mono::mono_method_invoker* terminate_;    
};

} // namespace engine
