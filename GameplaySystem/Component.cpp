#include "pch.h"
#include "Component.h"
#include "../monowrapper/monopp/mono_assembly.h"

mono::mono_method_invoker* Component::initialize_;
mono::mono_method_invoker* Component::terminate_;
mono::mono_method_invoker* Component::fixed_update_;
mono::mono_method_invoker* Component::update_;
mono::mono_method_invoker* Component::render_;

Component::Component(mono::mono_object* object_ref)
{
    object_reference = object_ref;
}

void Component::Initialize()
{
    initialize_->invoke(*object_reference);
}

void Component::FixedUpdate()
{
    fixed_update_->invoke(*object_reference);
}

void Component::Update()
{
    update_->invoke(*object_reference);
}

void Component::Render()
{
    render_->invoke(*object_reference);
}

void Component::Terminate()
{
    terminate_->invoke(*object_reference);
}

const mono::mono_object& Component::GetInternalPtr()
{
    return *object_reference;
}

void Component::CacheMethods(const mono::mono_assembly& assembly)
{
    mono::mono_type type = assembly.get_type("GameplayCore.Components", "Component");
    
    mono::mono_method initialize_method(type, "Initialize", 0);
    initialize_ = new mono::mono_method_invoker(initialize_method);

    mono::mono_method terminate_method(type, "Terminate", 0);
    terminate_ = new mono::mono_method_invoker(terminate_method);

    mono::mono_method fixed_update_method(type, "FixedUpdate", 0);
    fixed_update_ = new mono::mono_method_invoker(fixed_update_method);

    mono::mono_method update_method(type, "Update", 0);
    update_ = new mono::mono_method_invoker(update_method);

    mono::mono_method render_method(type, "Render", 0);
    render_ = new mono::mono_method_invoker(render_method);
}
