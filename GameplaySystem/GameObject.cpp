#include "pch.h"
#include "GameObject.h"
#include "Scene.h"
#include "Component.h"

namespace engine {

Property* GameObject::name_ = nullptr;
Property* GameObject::count_ = nullptr;
Method* GameObject::get_item_ = nullptr;

Method* GameObject::add_component_ = nullptr;
Method* GameObject::remove_component_ = nullptr;
Method* GameObject::get_component_ = nullptr;

Method* GameObject::initialize_ = nullptr;
Method* GameObject::terminate_ = nullptr;
Method* GameObject::fixed_update_ = nullptr;
Method* GameObject::update_ = nullptr;
Method* GameObject::render_ = nullptr;
Method* GameObject::debug_render_ = nullptr;
Method* GameObject::invalidate_ = nullptr;

const char kIsNotCachedErrorMessage[] = "GameObject methods are not cached.";

std::string GameObject::Name() const {
    assert(name_ != nullptr && kIsNotCachedErrorMessage);

    auto result = name_->GetValue(*this).value();
    mono::mono_string name(result.GetInternal());
    return name.as_utf8();
}

void GameObject::Name(std::string& value) {
    assert(name_ != nullptr && kIsNotCachedErrorMessage);

    mono::mono_string name(Runtime::GetCurrentRuntime().GetDomain(), value);
    name_->SetValue(GetInternal(), name.get_internal_ptr());
}

size_t GameObject::Count() const {
    assert(count_ != nullptr && kIsNotCachedErrorMessage);

    auto result = count_->GetValue(*this).value();
    int count = result.Unbox<int>();
    return static_cast<size_t>(count);
}

GameObject::GameObject(const Object& other)
    : Object(other)
{}

GameObject::GameObject(Object&& other)
    : Object(std::forward<Object>(other))
{}

std::shared_ptr<Component> GameObject::AddComponent(const TypeData& type_data)
{
    return AddComponent(Runtime::GetCurrentRuntime().GetType(type_data));
}

std::shared_ptr<Component> GameObject::AddComponent(const std::string& name_space, const std::string& name) {
    return AddComponent(Runtime::GetCurrentRuntime().GetType(name_space, name));
}

std::shared_ptr<Component> GameObject::AddComponent(const std::string& full_name) {
    return AddComponent(Runtime::GetCurrentRuntime().GetType(full_name));
}

std::shared_ptr<Component> GameObject::AddComponent(const Type& type) {
    assert(add_component_ != nullptr && kIsNotCachedErrorMessage);

    void* params[1];
    params[0] = type.GetInternal().get_internal_reflection_type_ptr();

    auto result = add_component_->Invoke(*this, params).value();
    auto component = std::make_shared<Component>(std::move(result));

    ComponentAdded.Broadcast(*this, component);
    return component;
}

void GameObject::RemoveComponent(std::shared_ptr<Component> component) {
    assert(remove_component_ != nullptr && kIsNotCachedErrorMessage);

    void* params[1];
    params[0] = component->GetType().GetInternal().get_internal_reflection_type_ptr();

    remove_component_->Invoke(GetInternal(), params);

    ComponentRemoved.Broadcast(*this, component);
    component.reset();
}

std::shared_ptr<Component> GameObject::GetComponent(const TypeData& type_data)
{
    return GetComponent(Runtime::GetCurrentRuntime().GetType(type_data));
}

std::shared_ptr<Component> GameObject::GetComponent(const std::string& name_space, const std::string& name) {
    return GetComponent(Runtime::GetCurrentRuntime().GetType(name_space, name));
}

std::shared_ptr<Component> GameObject::GetComponent(const std::string& full_name) {
    return GetComponent(Runtime::GetCurrentRuntime().GetType(full_name));
}

std::shared_ptr<Component> GameObject::GetComponent(const Type& type) {
    assert(get_component_ != nullptr && kIsNotCachedErrorMessage);

    void* params[1];
    params[0] = type.GetInternal().get_internal_reflection_type_ptr();

    auto result = get_component_->Invoke(*this, params);

    if (!result.has_value()) {
        return nullptr;
    }

    auto component = std::make_shared<Component>(result.value());
    return component;
}

void GameObject::Initialize() {
    assert(initialize_ != nullptr && kIsNotCachedErrorMessage);

    initialize_->Invoke(*this);
}

void GameObject::FixedUpdate() {
    assert(fixed_update_ != nullptr && kIsNotCachedErrorMessage);

    fixed_update_->Invoke(*this);
}

void GameObject::Update() {
    assert(update_ != nullptr && kIsNotCachedErrorMessage);

    update_->Invoke(*this);
}

void GameObject::Render() {
    assert(render_ != nullptr && kIsNotCachedErrorMessage);

    render_->Invoke(*this);
}

void GameObject::DebugRender() {
    assert(debug_render_ != nullptr && kIsNotCachedErrorMessage);

    debug_render_->Invoke(*this);
}

void GameObject::Terminate() {
    assert(terminate_ != nullptr && kIsNotCachedErrorMessage);

    terminate_->Invoke(*this);
}

void GameObject::Invalidate() {
    assert(invalidate_ != nullptr && kIsNotCachedErrorMessage);

    invalidate_->Invoke(*this);
}

std::shared_ptr<Component> GameObject::operator[](size_t index) const {
    assert(get_item_ != nullptr && kIsNotCachedErrorMessage);

    void* args[1];
    args[0] = &index;

    auto result = get_item_->Invoke(*this, args).value();
    auto component = std::make_shared<Component>(result);
    return component;
}

void GameObject::CacheMethods(const Runtime& runtime) {
    auto type = runtime.GetType(Types::kGameObject);

    name_ = new Property(type, "Name");
    count_ = new Property(type, "Count");

    get_item_ = new Method(type, "get_Item", 1);
    add_component_ = new Method(type, "AddComponent", 1);
    remove_component_ = new Method(type, "RemoveComponent", 1);
    get_component_ = new Method(type, "GetComponent", 1);
    initialize_ = new Method(type, "Initialize", 0);
    terminate_ = new Method(type, "Terminate", 0);
    fixed_update_ = new Method(type, "FixedUpdate", 0);
    update_ = new Method(type, "Update", 0);
    render_ = new Method(type, "Render", 0);
    debug_render_ = new Method(type, "DebugRender", 0);
    invalidate_ = new Method(type, "Invalidate", 0);
}

} // namespace engine
