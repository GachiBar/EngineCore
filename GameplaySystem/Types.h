#pragma once

#include <string>

namespace engine {

struct TypeData {
	const std::string name_space;
	const std::string name;
	const std::string full_name;

	TypeData(std::string name_space, std::string name)
		: name_space(name_space)
		, name(name)
		, full_name(name_space + "." + name)
	{}
};

struct Types {
	// Components
	static const TypeData kTransformComponent;

	// Components types
	static const TypeData kSingle;
	static const TypeData kDouble;
	static const TypeData kBoolean;
	static const TypeData kSByte;
	static const TypeData kInt16;
	static const TypeData kInt32;
	static const TypeData kInt64;
	static const TypeData kByte;
	static const TypeData kUInt16;
	static const TypeData kUInt32;
	static const TypeData kUInt64;
	static const TypeData kVector2;
	static const TypeData kVector3;
	static const TypeData kVector4;
	static const TypeData kString;
	static const TypeData kGameObject;

	// Attributes
	static const TypeData kHideInInspectorAttribute;
	static const TypeData kSerializeFieldAttribute;
	static const TypeData kInspectorNameAttribute;
	static const TypeData kSliderAttribute;
};

} // namespace engine