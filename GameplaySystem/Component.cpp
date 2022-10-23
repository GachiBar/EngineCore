#include "pch.h"
#include "Component.h"
#include "../monowrapper/monopp/mono_assembly.h"

mono::mono_method_invoker* Component::initialize_;
mono::mono_method_invoker* Component::terminate_;
mono::mono_method_invoker* Component::fixed_update_;
mono::mono_method_invoker* Component::update_;
mono::mono_method_invoker* Component::render_;

Component::Component(mono::mono_object object)
    : object_(std::move(object))
{}

void Component::Initialize()
{
    assert(initialize_ != nullptr && "Component methods are not cached.");
    initialize_->invoke(object_);
}

void Component::FixedUpdate()
{
    assert(fixed_update_ != nullptr && "Component methods are not cached.");
    fixed_update_->invoke(object_);
}

void Component::Update()
{
    assert(update_ != nullptr && "Component methods are not cached.");
    update_->invoke(object_);
}

void Component::Render()
{
    assert(render_ != nullptr && "Component methods are not cached.");
    render_->invoke(object_);
}

void Component::Terminate()
{
    assert(terminate_ != nullptr && "Component methods are not cached.");
    terminate_->invoke(object_);
}

const mono::mono_object& Component::GetInternal()
{
    return object_;
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
