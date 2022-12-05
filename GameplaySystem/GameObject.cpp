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

    mono::mono_string name(runtime_.GetDomain(), value);
    name_->SetValue(GetInternal(), name.get_internal_ptr());
}

size_t GameObject::Count() const {
    assert(count_ != nullptr && kIsNotCachedErrorMessage);

    auto result = count_->GetValue(*this).value();
    int count = result.Unbox<int>();
    return static_cast<size_t>(count);
}

GameObject::GameObject(const Runtime& runtime, mono::mono_object object)
    : Object(std::move(object))
    , runtime_(runtime)
{}

std::shared_ptr<Component> GameObject::AddComponent(const TypeData& type_data)
{
    return AddComponent(runtime_.GetType(type_data).GetInternal());
}

std::shared_ptr<Component> GameObject::AddComponent(const std::string& name_space, const std::string& name) {
    return AddComponent(runtime_.GetType(name_space, name).GetInternal());
}

std::shared_ptr<Component> GameObject::AddComponent(const mono::mono_type& component_type) {
    assert(add_component_ != nullptr && kIsNotCachedErrorMessage);

    MonoReflectionType* reflection_type = component_type.get_internal_reflection_type_ptr();
    void* params[1] = { reflection_type };

    auto component = add_component_->Invoke(*this, params).value();
    auto result = std::make_shared<Component>(runtime_, component.GetInternal());

    ComponentAdded.Broadcast(*this, result);
    return result;
}

void GameObject::RemoveComponent(std::shared_ptr<Component> component) {
    assert(remove_component_ != nullptr && kIsNotCachedErrorMessage);

    MonoReflectionType* reflection_type = component->GetInternal().get_type().get_internal_reflection_type_ptr();
    void* params[1] = { reflection_type };

    remove_component_->Invoke(GetInternal(), params);

    ComponentRemoved.Broadcast(*this, component);
    component.reset();
}

std::shared_ptr<Component> GameObject::GetComponent(const TypeData& type_data)
{
    return GetComponent(runtime_.GetType(type_data).GetInternal());
}

std::shared_ptr<Component> GameObject::GetComponent(const std::string& name_space, const std::string& name) {
    return GetComponent(runtime_.GetType(name_space, name).GetInternal());
}

std::shared_ptr<Component> GameObject::GetComponent(const mono::mono_type& component_type) {
    MonoReflectionType* reflection_type = component_type.get_internal_reflection_type_ptr();
    void* params[1] = { reflection_type };

    auto result = get_component_->Invoke(*this, params);

    if (!result.has_value()) {
        return nullptr;
    }

    return std::make_shared<Component>(runtime_, result.value().GetInternal());
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
    return std::make_shared<Component>(runtime_, result.GetInternal());
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
    invalidate_ = new Method(type, "Invalidate", 0);
}

} // namespace engine
