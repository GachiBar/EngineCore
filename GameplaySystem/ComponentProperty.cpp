#include "pch.h"
#include "ComponentProperty.h"
#include "Component.h"

namespace engine {

PropertyType ComponentProperty::GetType() {
	return type_;
}

std::string ComponentProperty::GetName() {
	return name_;
}

std::optional<mono::mono_object> ComponentProperty::GetValue() {
	auto raw_value = property_invoker_.get_value(component_.GetInternal());

	if (raw_value != nullptr) {
		mono::mono_object value(property_invoker_.get_value(component_.GetInternal()));
		return value;
	}

	return {};
}

void ComponentProperty::SetValue(void* data) {
	property_invoker_.set_value(component_.GetInternal(), data);
}

void ComponentProperty::SetValue(const mono::mono_object& value) {
	SetValue(value.get_internal_ptr());
}

ComponentProperty::ComponentProperty(const Component& component, std::string name)
	: component_(component)
	, name_(name)	
	, property_(component.GetInternal().get_type().get_property(name))
	, property_invoker_(property_)
	, type_(NameToPropertyType(property_.get_type().get_name()))
{}

PropertyType NameToPropertyType(const std::string& name) {
	if (name == "Single")		return PropertyType::kFloat;
	if (name == "Double")		return PropertyType::kDouble;
	if (name == "Boolean")		return PropertyType::kBool;
	if (name == "SByte")		return PropertyType::kByte;
	if (name == "Int16")		return PropertyType::kShort;
	if (name == "Int32")		return PropertyType::kInt;
	if (name == "Int64")		return PropertyType::kLong;
	if (name == "Byte")			return PropertyType::kUByte;
	if (name == "UInt16")		return PropertyType::kUShort;
	if (name == "UInt32")		return PropertyType::kUInt;
	if (name == "UInt64")		return PropertyType::kULong;
	if (name == "Vector2")		return PropertyType::kVector2;
	if (name == "Vector3")		return PropertyType::kVector3;
	if (name == "Vector4")		return PropertyType::kVector4;
	if (name == "String")		return PropertyType::kString;
	if (name == "GameObject")	return PropertyType::kGameObject;

	return PropertyType::kUndefined;
}

} // namespace engine
