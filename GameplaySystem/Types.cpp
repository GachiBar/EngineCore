#include "pch.h"
#include "Types.h"

namespace engine {

const TypeData Types::kUndefined("Undefined", "Undefined", Type::kUndefined);

// System
const TypeData Types::kSingle("System", "Single", Type::kFloat);
const TypeData Types::kDouble("System", "Double", Type::kDouble);
const TypeData Types::kBoolean("System", "Boolean", Type::kBool);
const TypeData Types::kSByte("System", "SByte", Type::kByte);
const TypeData Types::kInt16("System", "Int16", Type::kShort);
const TypeData Types::kInt32("System", "Int32", Type::kInt);
const TypeData Types::kInt64("System", "Int64", Type::kLong);
const TypeData Types::kByte("System", "Byte", Type::kUByte);
const TypeData Types::kUInt16("System", "UInt16", Type::kUShort);
const TypeData Types::kUInt32("System", "UInt32", Type::kUInt);
const TypeData Types::kUInt64("System", "UInt64", Type::kULong);
const TypeData Types::kVector2("GameplayCore.Mathematics", "Vector2", Type::kVector2);
const TypeData Types::kVector3("GameplayCore.Mathematics", "Vector3", Type::kVector3);
const TypeData Types::kVector4("GameplayCore.Mathematics", "Vector4", Type::kVector4);
const TypeData Types::kString("System", "String", Type::kString);

// GameplayCore
const TypeData Types::kGameObject("GameplayCore", "GameObject", Type::kGameObject);

// GameplayCore.Components
const TypeData Types::kTransformComponent("GameplayCore.Components", "TransformComponent", Type::kTransformComponent);

// Attributes
const TypeData Types::kHideInInspectorAttribute("GameplayCore.Serialization", "HideInInspectorAttribute", Type::kHideInInspectorAttribute);
const TypeData Types::kSerializeFieldAttribute("GameplayCore.Serialization", "SerializeFieldAttribute", Type::kSerializeFieldAttribute);
const TypeData Types::kInspectorNameAttribute("GameplayCore.Editor", "InspectorNameAttribute", Type::kInspectorNameAttribute);
const TypeData Types::kSliderAttribute("GameplayCore.Editor", "SliderAttribute", Type::kSliderAttribute);

const std::vector<const TypeData*> Types::types_data_ = {
	&kSingle,
	&kDouble,
	&kBoolean,
	&kSByte,
	&kInt16,
	&kInt32,
	&kInt64,
	&kByte,
	&kUInt16,
	&kUInt32,
	&kUInt64,
	&kVector2,
	&kVector3,
	&kVector4,
	&kString,
	&kGameObject,

	&kTransformComponent,

	&kHideInInspectorAttribute,
	&kSerializeFieldAttribute,
	&kInspectorNameAttribute,
	&kSliderAttribute,
};

const TypeData& Types::GetTypeData(std::string name_space, std::string name) {
	for (const auto* data : Types::types_data_) {
		if (data->name_space == name_space && data->name == name) {
			return *data;
		}
	}

	return kUndefined;
}

const TypeData& Types::GetTypeData(std::string full_name) {
	for (const auto* data : Types::types_data_) {
		if (data->full_name == full_name) {
			return *data;
		}
	}

	return kUndefined;
}

const TypeData& Types::GetTypeData(Type type) {
	for (const auto* data : Types::types_data_) {
		if (data->type == type) {
			return *data;
		}
	}

	return kUndefined;
}

} // namespace engine