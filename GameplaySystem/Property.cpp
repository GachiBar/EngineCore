#include "pch.h"
#include "Property.h"
#include "Attribute.h"

#include <mono/metadata/object.h>

namespace engine {

const mono::mono_property& Property::GetInternal() const {
	return property_;
}

std::vector<Attribute> Property::GetAttributes() const {
	std::vector<Attribute> attributes;
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

PropertyType Property::GetType() const {
	return type_;
}

std::string Property::GetName() const {
	return property_.get_name();
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

	if (raw_value != nullptr) {
		mono::mono_object value(property_invoker_.get_value(object_.GetInternal()));
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
	, type_(NameToPropertyType(property_.get_type().get_name()))
{}

PropertyType NameToPropertyType(const std::string& name) {
	if (name == "Single"	)	return PropertyType::kFloat;
	if (name == "Double"	)	return PropertyType::kDouble;
	if (name == "Boolean"	)	return PropertyType::kBool;
	if (name == "SByte"		)	return PropertyType::kByte;
	if (name == "Int16"		)	return PropertyType::kShort;
	if (name == "Int32"		)	return PropertyType::kInt;
	if (name == "Int64"		)	return PropertyType::kLong;
	if (name == "Byte"		)	return PropertyType::kUByte;
	if (name == "UInt16"	)	return PropertyType::kUShort;
	if (name == "UInt32"	)	return PropertyType::kUInt;
	if (name == "UInt64"	)	return PropertyType::kULong;
	if (name == "Vector2"	)	return PropertyType::kVector2;
	if (name == "Vector3"	)	return PropertyType::kVector3;
	if (name == "Vector4"	)	return PropertyType::kVector4;
	if (name == "String"	)	return PropertyType::kString;
	if (name == "GameObject")	return PropertyType::kGameObject;

	return PropertyType::kUndefined;
}

} // namespace engine
