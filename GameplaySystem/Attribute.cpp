#include "pch.h"
#include "Attribute.h"
#include "Types.h"

namespace engine {

AttributeType Attribute::GetType() const {
	return type_;
}

Attribute::Attribute(mono::mono_object object)
	: Object(std::move(object))
	, type_(NameToAttributeType(GetInternal().get_type().get_fullname()))
{}

AttributeType NameToAttributeType(const std::string& name) {
	if (name == Types::kHideInInspectorAttribute.full_name) {
		return AttributeType::kHideInInspectorAttribute;
	}		
	if (name == Types::kSerializeFieldAttribute.full_name) {
		return AttributeType::kSerializeFieldAttribute;
	}		
	if (name == Types::kInspectorNameAttribute.full_name) {
		return AttributeType::kInspectorNameAttribute;
	}		
	if (name == Types::kSliderAttribute.full_name) {
		return AttributeType::kSliderAttribute;
	}		

	return AttributeType::kUndefined;
}

} // namespace engine


