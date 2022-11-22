#include "pch.h"
#include "Property.h"
#include "Object.h"

#include <mono/metadata/object.h>

namespace engine {

const mono::mono_property& Property::GetInternal() const {
	return property_;
}

const std::string& Property::GetName() const {
	return property_.get_name();
}

const TypeData& Property::GetTypeData() const {
	return type_data_;
}

std::vector<Object> Property::GetAttributes() const {
	std::vector<Object> attributes;
	auto component_class = object_.GetInternal().get_type().get_internal_ptr();
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
	return { object_, property_.get_get_method() };
}

Method Property::GetSetMethod() const {
	return { object_, property_.get_set_method() };
}

std::optional<Object> Property::GetValue() {
	auto raw_value = property_invoker_.get_value(object_.GetInternal());

	if (raw_value.has_value()) {
		mono::mono_object value(raw_value.value());
		return { value };
	}

	return {};
}

void Property::SetValue(void* data) {
	property_invoker_.set_value(object_.GetInternal(), data);
}

void Property::SetValue(const mono::mono_object& value) {
	SetValue(value.get_internal_ptr());
}

void Property::SetValue(const Object& value) {
	SetValue(value.GetInternal());
}

Property::Property(const Object& object, const std::string& property_name)
	: Property(object, object.GetInternal().get_type().get_property(property_name))
{}

Property::Property(const Object& object, mono::mono_property property)
	: object_(object)
	, property_(std::move(property))
	, property_invoker_(property_)
	, type_data_(Types::GetTypeData(property_.get_type().get_fullname()))
{}

} // namespace engine
