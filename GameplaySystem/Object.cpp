#include "pch.h"
#include "Object.h"
#include "Property.h"

namespace engine {

const mono::mono_object& Object::GetInternal() const {
    return object_;
}

const mono::mono_type& Object::GetType() const {
    return type_;
}

const mono::mono_type& Object::GetBaseType() const {
    return type_.get_base_type();
}

const TypeData& Object::GetTypeData() const {
    return type_data_;
}

const std::string& Object::GetNameSpace() const {
    return type_.get_namespace();
}

const std::string& Object::GetName() const {
    return type_.get_name();
}

const std::string& Object::GetFullName() const {
    return type_.get_fullname();
}

Field Object::GetField(const std::string& name) const {
    return Field(*this, name);
}

std::vector<Field> Object::GetFields() const {
    std::vector<Field> fields;

    for (auto field : type_.get_fields()) {
        fields.emplace_back(*this, field);
    }

    return fields;
}

Property Object::GetProperty(const std::string& name) const {
    return Property(*this, name);
}

std::vector<Property> Object::GetProperties() const {
    std::vector<Property> properties;

    for (auto property : type_.get_properties()) {
        properties.emplace_back(*this, property);
    }

    return properties;
}

bool Object::HasMethod(const std::string& name) const {
    auto raw_method = mono_class_get_method_from_name(type_.get_internal_ptr(), name.c_str(), 0);
    return raw_method != nullptr;
}

bool Object::HasMethod(const std::string& name, int argc) const {
    auto raw_method = mono_class_get_method_from_name(type_.get_internal_ptr(), name.c_str(), argc);
    return raw_method != nullptr;
}

Method Object::GetMethod(const std::string& name) const {
    return { *this, type_.get_method(name) };
}

Method Object::GetMethod(const std::string& name, int argc) const {
    return { *this, type_.get_method(name, argc) };
}

std::vector<Method> Object::GetMethods() const {
    std::vector<Method> methods;

    for (auto method : type_.get_methods()) {
        methods.emplace_back(*this, method);
    }

    return methods;
}

Object::Object(MonoObject* mono_object)
    : Object(mono::mono_object(mono_object))
{}

Object::Object(mono::mono_object object)
    : Object(std::move(object), object.get_type())
{}

Object::Object(MonoObject* mono_object, MonoClass* mono_class)
    : Object(std::move(mono::mono_object(mono_object)), std::move(mono::mono_type(mono_class)))
{}

Object::Object(mono::mono_object object, mono::mono_type type)
    : object_(std::move(object))
    , type_(std::move(type))
    , handle_(mono_gchandle_new(object_.get_internal_ptr(), true))
    , type_data_(Types::GetTypeData(GetFullName()))
{
    assert(object_.get_type().is_derived_from(type_) && "Object is not type/not derived from type.");
}

Object::~Object() {
    mono_gchandle_free(handle_);
}

Object Object::ToBase() const
{
    return Object(object_, type_.get_base_type());
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
