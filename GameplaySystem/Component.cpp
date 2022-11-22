#include "pch.h"
#include "Component.h"
#include "GameObject.h"
#include "../monowrapper/monopp/mono_assembly.h"
#include "../monowrapper/monopp/mono_property.h"
#include "../monowrapper/monopp/mono_string.h"
#include "../monowrapper/monopp/mono_array.h"

namespace engine {

mono::mono_property_invoker* Component::game_object_ = nullptr;

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

void Component::CacheMethods(const mono::mono_assembly& assembly) {
    mono::mono_type type = assembly.get_type("GameplayCore.Components", "Component");

    mono::mono_property game_object_property(type, "GameObject");
    game_object_ = new mono::mono_property_invoker(game_object_property);
}

} // namespace engine
