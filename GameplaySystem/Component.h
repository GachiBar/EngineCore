#pragma once

#include "../monowrapper/monopp/mono_object.h"
#include "../monowrapper/monopp/mono_method_invoker.h"

namespace engine {

class Component {
public:
    const mono::mono_object& GetInternal() const;

    Component(mono::mono_object object);
    ~Component();

    void Initialize();
    void FixedUpdate();
    void Update();
    void Render();
    void Terminate();

    std::vector<std::string> GetEditablePropertiesNames();

    static void CacheMethods(const mono::mono_assembly& assembly);

private:
    uint32_t handle_;

    mono::mono_object object_;

    static mono::mono_method_invoker* initialize_;
    static mono::mono_method_invoker* fixed_update_;
    static mono::mono_method_invoker* update_;
    static mono::mono_method_invoker* render_;
    static mono::mono_method_invoker* terminate_;
    static mono::mono_method_invoker* get_editable_properties_names_;   
};

} // namespace engine
