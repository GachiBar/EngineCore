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

Object::Object(const Object& other) 
    : object_(other.GetInternal())
    , type_(other.GetType())
    , handle_(mono_gchandle_new(object_.get_internal_ptr(), true))
{}

Object::Object(Object&& other) noexcept
    : object_(other.GetInternal())
    , type_(other.GetType())
    , handle_(0)
{
    swap(*this, other);
}

Object::~Object() {
    if (handle_ != 0) {
        mono_gchandle_free(handle_);
    }    
}

void* Object::Unbox() const {
    return object_.unbox();
}

Object& Object::operator= (Object other) {
    swap(*this, other);
    return *this;
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

void swap(Object& first, Object& second) {
    using std::swap;

    swap(first.object_, second.object_);
    swap(first.type_, second.type_);
    swap(first.handle_, second.handle_);
}

} // namespace engine
