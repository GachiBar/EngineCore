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

Component::Component(MonoObject* mono_object)
    : Object(mono_object)
    , invalidate_(GetInvalidateMethod())
{}

Component::Component(const mono::mono_object& object)
    : Object(object)
    , invalidate_(GetInvalidateMethod())
{}

Component::Component(const Object& other)
    : Object(other)
    , invalidate_(GetInvalidateMethod())
{}

Component::Component(Object&& other) noexcept
    : Object(std::forward<Object>(other))
    , invalidate_(GetInvalidateMethod())
{}

void Component::Invalidate(const std::string& fieldName) {
    auto& domain = mono::mono_domain::get_current_domain();
    mono::mono_string field(domain, fieldName);

    void* params[1];
    params[0] = field.get_internal_ptr();

    invalidate_.Invoke(*this, params);
}

void Component::CacheMethods(const Runtime& runtime) {
    auto type = runtime.GetType(Types::kComponent);

    game_object_ = new Property(type, "GameObject");
}

Method Component::GetInvalidateMethod() {
    auto temp = GetType();

    while (!temp.HasMethod("Invalidate", 1)) {
        temp = temp.GetBaseType();
    }

    return temp.GetMethod("Invalidate", 1);
}

} // namespace engine
