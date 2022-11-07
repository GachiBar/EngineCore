#include "pch.h"
#include "Attribute.h"

namespace engine {

AttributeType Attribute::GetType() const {
	return type_;
}

Attribute::Attribute(mono::mono_object object)
	: Object(std::move(object))
	, type_(NameToAttributeType(GetInternal().get_type().get_fullname()))
{}

AttributeType NameToAttributeType(const std::string& name) {
	if (name == "GameplayCore.Serialization.HideInInspectorAttribute") return AttributeType::kHideInInspectorAttribute;
	if (name == "GameplayCore.Serialization.SerializeFieldAttribute" ) return AttributeType::kSerializeFieldAttribute;
	if (name == "GameplayCore.Editor.InspectorNameAttribute"		 ) return AttributeType::kInspectorNameAttribute;
	if (name == "GameplayCore.Editor.SliderAttribute"				 ) return AttributeType::kSliderAttribute;

	return AttributeType::kUndefined;
}

} // namespace engine


