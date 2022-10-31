#include "pch.h"
#include "Component.h"
#include "../monowrapper/monopp/mono_assembly.h"
#include "../monowrapper/monopp/mono_property.h"
#include "../monowrapper/monopp/mono_string.h"
#include "../monowrapper/monopp/mono_array.h"

namespace engine {

mono::mono_method_invoker* Component::initialize_;
mono::mono_method_invoker* Component::terminate_;
mono::mono_method_invoker* Component::fixed_update_;
mono::mono_method_invoker* Component::update_;
mono::mono_method_invoker* Component::render_;
mono::mono_method_invoker* Component::get_editable_properties_names_;

const char kIsNotCachedErrorMessage[] = "Component methods are not cached.";

const mono::mono_object& Component::GetInternal() const {
    return object_;
}

Component::Component(mono::mono_object object)
    : object_(std::move(object))
{
    mono::mono_type type = object_.get_type();
}

void Component::Initialize() {
    assert(initialize_ != nullptr && kIsNotCachedErrorMessage);

    initialize_->invoke(object_);
}

void Component::FixedUpdate() {
    assert(fixed_update_ != nullptr && kIsNotCachedErrorMessage);

    fixed_update_->invoke(object_);
}

void Component::Update() {
    assert(update_ != nullptr && kIsNotCachedErrorMessage);

    update_->invoke(object_);
}

void Component::Render() {
    assert(render_ != nullptr && kIsNotCachedErrorMessage);

    render_->invoke(object_);
}

void Component::Terminate() {
    assert(terminate_ != nullptr && kIsNotCachedErrorMessage);

    terminate_->invoke(object_);
}

std::vector<std::string> Component::GetEditablePropertiesNames() {
    std::vector<std::string> names;
    auto raw_object = get_editable_properties_names_->invoke(object_);
    auto raw_array = (MonoArray*)raw_object;
    mono::mono_array properties_names(raw_array);

    for (int i = 0; i < properties_names.get_length(); ++i) {
        mono::mono_object name_object(properties_names.get(i));
        mono::mono_string name_string(name_object);
        std::string name = name_string.as_utf8();
        names.push_back(name);
    }

    return names;
}

void Component::CacheMethods(const mono::mono_assembly& assembly) {
    mono::mono_type type = assembly.get_type("GameplayCore.Components", "Component");

    mono::mono_method get_editable_properties_names_method(type, "GetEditablePropertiesNames", 0);
    get_editable_properties_names_ = new mono::mono_method_invoker(get_editable_properties_names_method);

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
