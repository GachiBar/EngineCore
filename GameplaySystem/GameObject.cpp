﻿#include "pch.h"
#include "GameObject.h"
#include "Scene.h"
#include "Component.h"
#include "../monowrapper/monopp/mono_assembly.h"
#include "../monowrapper/monopp/mono_domain.h"
#include "../monowrapper/monopp/mono_string.h"

namespace engine {

mono::mono_property_invoker* GameObject::name_ = nullptr;
mono::mono_property_invoker* GameObject::count_ = nullptr;
mono::mono_method_invoker* GameObject::get_item_ = nullptr;

mono::mono_method_invoker* GameObject::add_component_ = nullptr;
mono::mono_method_invoker* GameObject::remove_component_ = nullptr;
mono::mono_method_invoker* GameObject::get_component_ = nullptr;

mono::mono_method_invoker* GameObject::initialize_ = nullptr;
mono::mono_method_invoker* GameObject::terminate_ = nullptr;
mono::mono_method_invoker* GameObject::fixed_update_ = nullptr;
mono::mono_method_invoker* GameObject::update_ = nullptr;
mono::mono_method_invoker* GameObject::render_ = nullptr;
mono::mono_method_invoker* GameObject::invalidate_ = nullptr;

const char kIsNotCachedErrorMessage[] = "GameObject methods are not cached.";

std::string GameObject::Name() const {
    assert(name_ != nullptr && kIsNotCachedErrorMessage);

    mono::mono_object value(name_->get_value(GetInternal()).value());
    mono::mono_string name(value);
    return name.as_utf8();
}

void GameObject::Name(std::string& value) {
    assert(name_ != nullptr && kIsNotCachedErrorMessage);

    auto& domain = mono::mono_domain::get_current_domain();
    mono::mono_string name(domain, value);
    name_->set_value(GetInternal(), name.get_internal_ptr());
}

size_t GameObject::Count() const {
    assert(count_ != nullptr && kIsNotCachedErrorMessage);

    mono::mono_object value(count_->get_value(GetInternal()).value());
    int count = *reinterpret_cast<int*>(value.unbox());
    
    assert(count >= 0 && "Count less then zero.");

    return static_cast<size_t>(count);
}

GameObject::GameObject(const mono::mono_assembly& assembly, mono::mono_object object)
    : Object(std::move(object))
    , assembly_(assembly)
{}

std::shared_ptr<Component> GameObject::AddComponent(const std::string& name_space, const std::string& name) {
    return AddComponent(assembly_.get_type(name_space, name));
}

std::shared_ptr<Component> GameObject::AddComponent(const mono::mono_type& component_type) {
    MonoReflectionType* reflection_type = component_type.get_internal_reflection_type_ptr();
    void* params[1] = { reflection_type };

    auto raw_value = add_component_->invoke(GetInternal(), params);
    mono::mono_object component(raw_value.value());
    auto result = std::make_shared<Component>(assembly_, std::move(component));

    ComponentAdded.Broadcast(*this, result);
    return result;
}

void GameObject::RemoveComponent(std::shared_ptr<Component> component) {
    MonoReflectionType* reflection_type = component->GetInternal().get_type().get_internal_reflection_type_ptr();
    void* params[1] = { reflection_type };

    remove_component_->invoke(GetInternal(), params);

    ComponentRemoved.Broadcast(*this, component);
    component.reset();
}

std::shared_ptr<Component> GameObject::GetComponent(const std::string& name_space, const std::string& name) {
    return GetComponent(assembly_.get_type(name_space, name));
}

std::shared_ptr<Component> GameObject::GetComponent(const mono::mono_type& component_type) {
    MonoReflectionType* reflection_type = component_type.get_internal_reflection_type_ptr();
    void* params[1] = { reflection_type };

    auto raw_component = get_component_->invoke(GetInternal(), params);

    if (!raw_component.has_value()) {
        return nullptr;
    }

    mono::mono_object component(raw_component.value());
    return std::shared_ptr<Component>(new Component(assembly_, std::move(component)));
}

void GameObject::Initialize() {
    assert(initialize_ != nullptr && kIsNotCachedErrorMessage);

    initialize_->invoke(GetInternal());
}

void GameObject::FixedUpdate() {
    assert(fixed_update_ != nullptr && kIsNotCachedErrorMessage);

    fixed_update_->invoke(GetInternal());
}

void GameObject::Update() {
    assert(update_ != nullptr && kIsNotCachedErrorMessage);

    update_->invoke(GetInternal());
}

void GameObject::Render() {
    assert(render_ != nullptr && kIsNotCachedErrorMessage);

    render_->invoke(GetInternal());
}

void GameObject::Terminate() {
    assert(terminate_ != nullptr && kIsNotCachedErrorMessage);

    terminate_->invoke(GetInternal());
}

void GameObject::Invalidate() {
    assert(invalidate_ != nullptr && kIsNotCachedErrorMessage);

    invalidate_->invoke(GetInternal());
}

std::shared_ptr<Component> GameObject::operator[](size_t index) const {
    assert(get_item_ != nullptr && kIsNotCachedErrorMessage);

    void* args[1];
    args[0] = &index;

    auto raw_value = get_item_->invoke(GetInternal(), args);
    mono::mono_object component(raw_value.value());
    return std::shared_ptr<Component>(new Component(assembly_, component));
}

void GameObject::CacheMethods(const mono::mono_assembly& assembly) {
    mono::mono_type type = assembly.get_type("GameplayCore", "GameObject");

    mono::mono_property name_property(type, "Name");
    name_ = new mono::mono_property_invoker(name_property);

    mono::mono_property count_property(type, "Count");
    count_ = new mono::mono_property_invoker(count_property);

    mono::mono_method get_item_method(type, "get_Item", 1);
    get_item_ = new mono::mono_method_invoker(get_item_method);

    mono::mono_method add_component_method(type, "AddComponent", 1);
    add_component_ = new mono::mono_method_invoker(add_component_method);

    mono::mono_method remove_component_method(type, "RemoveComponent", 1);
    remove_component_ = new mono::mono_method_invoker(remove_component_method);

    mono::mono_method get_component_method(type, "GetComponent", 1);
    get_component_ = new mono::mono_method_invoker(get_component_method);

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
}

} // namespace engine
