#include "pch.h"
#include "GameObject.h"
#include "Scene.h"
#include "Component.h"

#include "../monowrapper/monopp/mono_assembly.h"

mono::mono_method_invoker* GameObject::initialize_ = nullptr;
mono::mono_method_invoker* GameObject::terminate_ = nullptr;
mono::mono_method_invoker* GameObject::fixed_update_ = nullptr;
mono::mono_method_invoker* GameObject::update_ = nullptr;
mono::mono_method_invoker* GameObject::render_ = nullptr;
mono::mono_method_invoker* GameObject::invalidate_ = nullptr;
mono::mono_method_invoker* GameObject::add_component = nullptr;
mono::mono_method_invoker* GameObject::remove_component = nullptr;
mono::mono_method_invoker* GameObject::get_component = nullptr;

const mono::mono_object& GameObject::GetInternal() {
    return object_;
}

GameObject::GameObject(const mono::mono_assembly& assembly, mono::mono_object object)
    : assembly_(assembly)
    , object_(std::move(object))
{}

std::shared_ptr<Component> GameObject::AddComponent(const std::string& name_space, const std::string& name) {
    return AddComponent(assembly_.get_type(name_space, name));
}

std::shared_ptr<Component> GameObject::AddComponent(const mono::mono_type& component_type) {
    MonoReflectionType* reflection_type = component_type.get_internal_reflection_type_ptr();
    void* params[1] = {reflection_type};

    mono::mono_object component(add_component->invoke(object_, params));
    Invalidate();
    return std::shared_ptr<Component>(new Component(std::move(component)));
}

void GameObject::RemoveComponent(std::shared_ptr<Component> component) {
    MonoReflectionType* reflection_type = component->GetInternal().get_type().get_internal_reflection_type_ptr();
    void* params[1] = {reflection_type};

    remove_component->invoke(object_, params);
    Invalidate();
    component.reset();
}

std::shared_ptr<Component> GameObject::GetComponent(const std::string& name_space, const std::string& name) {
    return GetComponent(assembly_.get_type(name_space, name));
}

std::shared_ptr<Component> GameObject::GetComponent(const mono::mono_type& component_type) {
    MonoReflectionType* reflection_type = component_type.get_internal_reflection_type_ptr();
    void* params[1] = {reflection_type};

    mono::mono_object component(get_component->invoke(object_, params));
    return std::shared_ptr<Component>(new Component(std::move(component)));
}

void GameObject::Initialize() {
    assert(initialize_ != nullptr && "GameObject methods are not cached.");
    initialize_->invoke(object_);
}

void GameObject::FixedUpdate() {
    assert(fixed_update_ != nullptr && "GameObject methods are not cached.");
    fixed_update_->invoke(object_);
}

void GameObject::Update() {
    assert(update_ != nullptr && "GameObject methods are not cached.");
    update_->invoke(object_);
}

void GameObject::Render() {
    assert(render_ != nullptr && "GameObject methods are not cached.");
    render_->invoke(object_);
}

void GameObject::Terminate() {
    assert(terminate_ != nullptr && "GameObject methods are not cached.");
    terminate_->invoke(object_);
}

void GameObject::Invalidate() {
    assert(invalidate_ != nullptr && "GameObject methods are not cached.");
    invalidate_->invoke(object_);
}

void GameObject::CacheMethods(const mono::mono_assembly& assembly) {
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
