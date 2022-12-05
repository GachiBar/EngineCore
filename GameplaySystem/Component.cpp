#include "pch.h"
#include "Component.h"
#include "GameObject.h"

namespace engine {

Property* Component::game_object_ = nullptr;

const char kIsNotCachedErrorMessage[] = "Component methods are not cached.";

std::string Component::Name() const {
    return GetType().GetInternal().get_fullname();
}

std::shared_ptr<GameObject> Component::GameObject() {
    assert(game_object_ != nullptr && kIsNotCachedErrorMessage);

    auto result = game_object_->GetValue(*this).value();
    return std::make_shared<engine::GameObject>(result.GetInternal());
}

Component::Component(const Object& other)
    : Object(other)
{}

void Component::CacheMethods(const Runtime& runtime) {
    auto type = runtime.GetType(Types::kComponent);

    game_object_ = new Property(type, "GameObject");
}

} // namespace engine
