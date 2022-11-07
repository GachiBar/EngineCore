#pragma once

#include "Object.h"

namespace engine {

enum class AttributeType {
	kHideInInspectorAttribute,
	kSerializeFieldAttribute,
	kInspectorNameAttribute,
	kSliderAttribute,
	kUndefined,
};

class Attribute : public Object {
public:
	AttributeType GetType() const;

	Attribute(mono::mono_object object);

private:
	const AttributeType type_;
};

AttributeType NameToAttributeType(const std::string& name);

} //namespace engine