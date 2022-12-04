#include "pch.h"
#include "Object.h"
#include "Property.h"

namespace engine {

const mono::mono_object& Object::GetInternal() const {
    return object_;
}

const Type& Object::GetType() const {
    return type_;
}

Object::Object(MonoObject* mono_object)
    : Object(mono::mono_object(mono_object))
{}

Object::Object(const mono::mono_object& object)
    : object_(object)
    , type_(object_.get_type())
    , handle_(mono_gchandle_new(object_.get_internal_ptr(), true))
{}

Object::~Object() {
    mono_gchandle_free(handle_);
}

void* Object::Unbox() const {
    return object_.unbox();
}

bool operator== (const Object& lhs, const Object& rhs) {
    return lhs.GetInternal().get_internal_ptr() == rhs.GetInternal().get_internal_ptr();
}

bool operator== (const Object& lhs, const mono::mono_object& rhs) {
    return lhs.GetInternal().get_internal_ptr() == rhs.get_internal_ptr();
}

bool operator== (const mono::mono_object& lhs, const Object& rhs) {
    return lhs.get_internal_ptr() == rhs.GetInternal().get_internal_ptr();
}

} // namespace engine
