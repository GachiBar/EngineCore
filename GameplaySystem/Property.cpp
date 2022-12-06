#include "pch.h"
#include "Property.h"
#include "Method.h"
#include "Object.h"

#include <mono/metadata/object.h>

namespace engine {

const mono::mono_property& Property::GetInternal() const {
	return property_;
}

const std::string& Property::GetName() const {
	return property_.get_name();
}

const Type& Property::GetType() const {
	return property_type_;
}

std::vector<Object> Property::GetAttributes() const {
	std::vector<Object> attributes;
	auto component_class = class_type_.GetInternal().get_internal_ptr();
	auto property = property_.get_internal_ptr();
	auto ainfo = mono_custom_attrs_from_property(component_class, property);

	if (ainfo != nullptr) {
		for (size_t i = 0; i < ainfo->num_attrs; ++i) {
			auto centry = &ainfo->attrs[i];
			auto attribute_class = mono_method_get_class(centry->ctor);
			auto attribute = mono_custom_attrs_get_attr(ainfo, attribute_class);
			attributes.emplace_back(mono::mono_object(attribute));
		}
	}

	//mono_free(ainfo);
	return attributes;
}

bool Property::CanRead() const {
	auto getter = mono_property_get_get_method(property_.get_internal_ptr());
	return getter != nullptr;
}

bool Property::CanWrite() const {
	auto setter = mono_property_get_set_method(property_.get_internal_ptr());
	return setter != nullptr;
}

Method Property::GetGetMethod() const {
	return Method(class_type_, property_.get_get_method());
}

Method Property::GetSetMethod() const {
	return Method(class_type_, property_.get_set_method());
}

Property::Property(Type type, const std::string name)
	: Property(type, type.GetInternal().get_property(name))
{}

Property::Property(Type type, mono::mono_property property)
	: property_(std::move(property))
	, property_invoker_(property_)
	, class_type_(type)
	, property_type_(property_.get_type())
{}

std::optional<Object> Property::GetValue() {
	auto raw_value = property_invoker_.get_value();

	if (raw_value.has_value()) {
		return { raw_value.value() };
	}

	return {};
}

std::optional<Object> Property::GetValue(const Object& object) {
	auto raw_value = property_invoker_.get_value(object.GetInternal());

	if (raw_value.has_value()) {
		return { raw_value.value() };
	}

	return {};
}

void Property::SetValue(void* data) {
	property_invoker_.set_value(data);
}

void Property::SetValue(const Object& value) {
	SetValue(value.GetInternal().get_internal_ptr());
}

void Property::SetValue(const Object& object, void* data) {
	property_invoker_.set_value(object.GetInternal(), data);
}

void Property::SetValue(const Object& object, const Object& value) {
	SetValue(object, value.GetInternal().get_internal_ptr());
}

} // namespace engine
