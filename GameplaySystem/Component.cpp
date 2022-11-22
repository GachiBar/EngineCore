#include "pch.h"
#include "Component.h"
#include "GameObject.h"
#include "../monowrapper/monopp/mono_assembly.h"
#include "../monowrapper/monopp/mono_property.h"
#include "../monowrapper/monopp/mono_string.h"
#include "../monowrapper/monopp/mono_array.h"

namespace engine {

mono::mono_property_invoker* Component::game_object_ = nullptr;

mono::mono_method_invoker* Component::initialize_ = nullptr;
mono::mono_method_invoker* Component::terminate_ = nullptr;
mono::mono_method_invoker* Component::fixed_update_ = nullptr;
mono::mono_method_invoker* Component::update_ = nullptr;
mono::mono_method_invoker* Component::render_ = nullptr;

const char kIsNotCachedErrorMessage[] = "Component methods are not cached.";

std::string Component::Name() const {
    return GetInternal().get_type().get_name();
}

std::shared_ptr<GameObject> Component::GameObject() {
    assert(game_object_ != nullptr && kIsNotCachedErrorMessage);

    auto raw_value = game_object_->get_value(GetInternal());
    mono::mono_object result(raw_value.value());
    return std::make_shared<engine::GameObject>(assembly_, result);
}

Component::Component(const mono::mono_assembly& assembly, mono::mono_object object)
    : Object(std::move(object))
    , assembly_(assembly)
{}

void Component::Initialize() {
    assert(initialize_ != nullptr && kIsNotCachedErrorMessage);

    initialize_->invoke(GetInternal());
}

void Component::FixedUpdate() {
    assert(fixed_update_ != nullptr && kIsNotCachedErrorMessage);

    fixed_update_->invoke(GetInternal());
}

void Component::Update() {
    assert(update_ != nullptr && kIsNotCachedErrorMessage);

    update_->invoke(GetInternal());
}

void Component::Render() {
    assert(render_ != nullptr && kIsNotCachedErrorMessage);

    render_->invoke(GetInternal());
}

void Component::Terminate() {
    assert(terminate_ != nullptr && kIsNotCachedErrorMessage);

    terminate_->invoke(GetInternal());
}

void Component::CacheMethods(const mono::mono_assembly& assembly) {
    mono::mono_type type = assembly.get_type("GameplayCore.Components", "Component");

    mono::mono_property game_object_property(type, "GameObject");
    game_object_ = new mono::mono_property_invoker(game_object_property);

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

} // namespace engine
