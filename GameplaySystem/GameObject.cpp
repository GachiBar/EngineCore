#include "pch.h"
#include "GameObject.h"
#include "Scene.h"
#include "Component.h"

#include "../monowrapper/monopp/mono_assembly.h"

mono::mono_method_invoker* GameObject::initialize_;
mono::mono_method_invoker* GameObject::terminate_;
mono::mono_method_invoker* GameObject::fixed_update_;
mono::mono_method_invoker* GameObject::update_;
mono::mono_method_invoker* GameObject::render_;
mono::mono_method_invoker* GameObject::invalidate_;
mono::mono_method_invoker* GameObject::add_component;
mono::mono_method_invoker* GameObject::remove_component;
mono::mono_method_invoker* GameObject::get_component;

GameObject::GameObject(mono::mono_object* object_ref)
{
    assert(object_ref->get_type().get_name() == "GameObject" && "Can't init game object with different class!");
    object_reference = object_ref;
}

std::shared_ptr<Component> GameObject::AddComponent(const mono::mono_assembly& assembly, const std::string& name_space, const std::string& name)
{
    return AddComponent(assembly.get_type(name_space, name));
}

std::shared_ptr<Component> GameObject::AddComponent(const mono::mono_type& component_type)
{
    MonoReflectionType* reflection_type = component_type.get_internal_reflection_type_ptr();
    void* params[1] = {reflection_type};

    auto component = new mono::mono_object(add_component->invoke(*object_reference, params));
    return std::make_shared<Component>(component);
}

void GameObject::RemoveComponent(std::shared_ptr<Component> component)
{
    MonoReflectionType* reflection_type = component->GetInternalPtr().get_type().get_internal_reflection_type_ptr();
    void* params[1] = {reflection_type};

    remove_component->invoke(*object_reference, params);
    component.reset();
}

std::shared_ptr<Component> GameObject::GetComponent(mono::mono_assembly& assembly, const std::string& name_space, const std::string& name)
{
    return GetComponent(assembly.get_type(name_space, name));
}

std::shared_ptr<Component> GameObject::GetComponent(const mono::mono_type& component_type)
{
    MonoReflectionType* reflection_type = component_type.get_internal_reflection_type_ptr();
    void* params[1] = {reflection_type};

    auto component = new mono::mono_object(get_component->invoke(*object_reference, params));
    return std::make_shared<Component>(component);
}

const mono::mono_object& GameObject::GetInternalPtr()
{
    return *object_reference;
}

void GameObject::Initialize()
{
    initialize_->invoke(*object_reference);
}

void GameObject::FixedUpdate()
{
    fixed_update_->invoke(*object_reference);
}

void GameObject::Update()
{
    update_->invoke(*object_reference);
}

void GameObject::Render()
{
    render_->invoke(*object_reference);
}

void GameObject::Terminate()
{
    terminate_->invoke(*object_reference);
}

void GameObject::Invalidate()
{
    invalidate_->invoke(*object_reference);
}

void GameObject::CacheMethods(const mono::mono_assembly& assembly)
{
    mono::mono_type type = assembly.get_type("GameplayCore", "GameObject");
    
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

    mono::mono_method invalidate_method(type, "Invalidate", 0);
    invalidate_ = new mono::mono_method_invoker(invalidate_method);

    mono::mono_method get_component_method(type, "GetComponent", 1);
    get_component = new mono::mono_method_invoker(get_component_method);

    mono::mono_method remove_component_method(type, "RemoveComponent", 1);
    remove_component = new mono::mono_method_invoker(remove_component_method);

    mono::mono_method add_component_method(type, "AddComponent", 1);
    add_component = new mono::mono_method_invoker(add_component_method);
}
