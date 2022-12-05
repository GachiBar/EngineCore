#include "pch.h"
#include "Type.h"
#include "Object.h"
#include "Method.h"

namespace engine {

const mono::mono_type& Type::GetInternal() const {
	return type_;
}

const TypeData& Type::GetTypeData() const {
	return *type_data_;
}

const bool Type::HasBaseType() const {
	return type_.has_base_type();
}

const Type Type::GetBaseType() const {
	return { type_.get_base_type() };
}

const std::string Type::GetName() const {
	return type_.get_name();
}

const std::string Type::GetNameSpace() const {
	return type_.get_namespace();
}

const std::string Type::GetFullName() const {
	return type_.get_fullname();
}

const bool Type::IsAbstract() const {
	return type_.is_abstract();
}

const bool Type::IsDerivedFrom(const Type& type) const {
	return type_.is_derived_from(type.GetInternal());
}

Field Type::GetField(const std::string& name) const {
	return Field(*this, name);
}

std::vector<Field> Type::GetFields() const {
	std::vector<Field> fields;

	for (auto field : type_.get_fields()) {
		fields.emplace_back(*this, field);
	}

	return fields;
}

Property Type::GetProperty(const std::string& name) const {
	return Property(*this, name);
}

std::vector<Property> Type::GetProperties() const {
	std::vector<Property> properties;

	for (auto property : type_.get_properties()) {
		properties.emplace_back(*this, property);
	}

	return properties;
}

bool Type::HasMethod(const std::string& name) const {
	return HasMethod(name, 0);
}

bool Type::HasMethod(const std::string& name, int argc) const {
	auto raw_method = mono_class_get_method_from_name(type_.get_internal_ptr(), name.c_str(), argc);
	return raw_method != nullptr;
}

Method Type::GetMethod(const std::string& name) const {
	return { *this, type_.get_method(name) };
}

Method Type::GetMethod(const std::string& name, int argc) const {
	return { *this, type_.get_method(name, argc) };
}

std::vector<Method> Type::GetMethods() const {
	std::vector<Method> methods;

	for (auto method : type_.get_methods()) {
		methods.emplace_back(*this, method);
	}

	return methods;
}

Type::Type(mono::mono_type type)
	: type_(type)
	, type_data_(&Types::GetTypeData(type_.get_fullname()))
{}

bool Type::Is(const TypeData& type_data) const {
	return *type_data_ == type_data;
}

Object Type::Instantiate() const {
	return { type_.new_instance() };
}

} // namespace engine