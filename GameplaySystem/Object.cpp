#include "pch.h"
#include "Object.h"
#include "Property.h"

namespace engine {

const mono::mono_object& engine::Object::GetInternal() const {
    return object_;
}

const TypeData& engine::Object::GetTypeData() const {
    return type_data_;
}

const std::string& Object::GetNameSpace() const {
    return object_.get_type().get_namespace();
}

const std::string& Object::GetName() const {
    return object_.get_type().get_name();
}

const std::string& Object::GetFullName() const {
    return object_.get_type().get_fullname();
}

Property Object::GetProperty(const std::string& name) const {
    return Property(*this, name);
}

std::vector<Property> Object::GetProperties() const {
    std::vector<Property> properties;

    for (auto property : GetInternal().get_type().get_properties()) {
        properties.emplace_back(*this, property);
    }

    return properties;
}

Method Object::GetMethod(const std::string& name) const {
    return { *this, object_.get_type().get_method(name) };
}

Method Object::GetMethod(const std::string& name, int argc) const {
    return { *this, object_.get_type().get_method(name, argc) };
}

std::vector<Method> Object::GetMethods() const {
    std::vector<Method> methods;

    for (auto method : GetInternal().get_type().get_methods()) {
        methods.emplace_back(*this, method);
    }

    return methods;
}

Object::Object(MonoObject* mono_object)
    : Object(mono::mono_object(mono_object))
{}

Object::Object(mono::mono_object object)
    : object_(object)
    , handle_(mono_gchandle_new(object_.get_internal_ptr(), true))
    , type_data_(Types::GetTypeData(GetFullName()))
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
