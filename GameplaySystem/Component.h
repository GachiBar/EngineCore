#pragma once

#include "../monowrapper/monopp/mono_object.h"
#include "../monowrapper/monopp/mono_method_invoker.h"

class Component {
    friend class GameObject;

public:   
    const mono::mono_object& GetInternal() const;

    void Initialize();
    void FixedUpdate();
    void Update();
    void Render();
    void Terminate();  
    
    static void CacheMethods(const mono::mono_assembly& assembly);

private:
    mono::mono_object object_;

    static mono::mono_method_invoker* initialize_;
    static mono::mono_method_invoker* fixed_update_;
    static mono::mono_method_invoker* update_;
    static mono::mono_method_invoker* render_;
    static mono::mono_method_invoker* terminate_;

    Component(mono::mono_object object);
};
