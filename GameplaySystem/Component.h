#pragma once

#include "../monowrapper/monopp/mono_object.h"
#include "../monowrapper/monopp/mono_method_invoker.h"

class Component
{
public:
    explicit Component(mono::mono_object* object_ref);
    
    void Initialize();
    void FixedUpdate();
    void Update();
    void Render();
    void Terminate();
    
    const mono::mono_object& GetInternalPtr();
    
    static void CacheMethods(const mono::mono_assembly& assembly);

private:
    mono::mono_object* object_reference;

    static mono::mono_method_invoker* initialize_;
    static mono::mono_method_invoker* fixed_update_;
    static mono::mono_method_invoker* update_;
    static mono::mono_method_invoker* render_;
    static mono::mono_method_invoker* terminate_;
};
