#pragma once

#include <string>
#include <vector>

namespace engine {

enum class Type {
	kUndefined,

	kFloat,
	kDouble,
	kBool,
	kByte,
	kShort,
	kInt,
	kLong,
	kUByte,
	kUShort,
	kUInt,
	kULong,
	kVector2,
	kVector3,
	kVector4,
	kString,

	kGameObject,

	kTransformComponent,

	kHideInInspectorAttribute,
	kSerializeFieldAttribute,
	kInspectorNameAttribute,
	kSliderAttribute,
};

struct TypeData {
	const std::string name_space;
	const std::string name;
	const std::string full_name;
	const Type type;

	TypeData(std::string name_space, std::string name, Type type)
		: name_space(name_space)
		, name(name)
		, full_name(name_space + "." + name)
		, type(type)
	{}
};

struct Types {		
public:
	static const TypeData kUndefined;

	// System
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

	// GameplayCore
	static const TypeData kGameObject;

	// GameplayCore.Components
	static const TypeData kTransformComponent;

	// Attributes
	static const TypeData kHideInInspectorAttribute;
	static const TypeData kSerializeFieldAttribute;
	static const TypeData kInspectorNameAttribute;
	static const TypeData kSliderAttribute;

	static const TypeData& GetTypeData(std::string name_space, std::string name);
	static const TypeData& GetTypeData(std::string full_name);
	static const TypeData& GetTypeData(Type type);

private:
	static const std::vector<const TypeData*> types_data_;
};



} // namespace engine