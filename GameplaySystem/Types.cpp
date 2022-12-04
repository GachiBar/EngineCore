#include "pch.h"
#include "Types.h"

#include <unordered_set>
#include <cassert>

namespace engine {

std::unordered_set<TypeData> registered_types_;

const TypeData& Types::kUndefined = Types::RegisterType("Undefined", "Undefined");

// System
const TypeData& Types::kObject = Types::RegisterType("System", "Object");
const TypeData& Types::kSingle = Types::RegisterType("System", "Single");
const TypeData& Types::kDouble = Types::RegisterType("System", "Double");
const TypeData& Types::kBoolean = Types::RegisterType("System", "Boolean");
const TypeData& Types::kSByte = Types::RegisterType("System", "SByte");
const TypeData& Types::kInt16 = Types::RegisterType("System", "Int16");
const TypeData& Types::kInt32 = Types::RegisterType("System", "Int32");
const TypeData& Types::kInt64 = Types::RegisterType("System", "Int64");
const TypeData& Types::kByte = Types::RegisterType("System", "Byte");
const TypeData& Types::kUInt16 = Types::RegisterType("System", "UInt16");
const TypeData& Types::kUInt32 = Types::RegisterType("System", "UInt32");
const TypeData& Types::kUInt64 = Types::RegisterType("System", "UInt64");
const TypeData& Types::kVector2 = Types::RegisterType("GameplayCore.Mathematics", "Vector2");
const TypeData& Types::kVector3 = Types::RegisterType("GameplayCore.Mathematics", "Vector3");
const TypeData& Types::kVector4 = Types::RegisterType("GameplayCore.Mathematics", "Vector4");
const TypeData& Types::kString = Types::RegisterType("System", "String");

// GameplayCore
const TypeData& Types::kGameObject = Types::RegisterType("GameplayCore", "GameObject");

// GameplayCore.Components
const TypeData& Types::kComponent = Types::RegisterType("GameplayCore.Components", "Component");
const TypeData& Types::kTransformComponent = Types::RegisterType("GameplayCore.Components", "TransformComponent");

// Attributes
const TypeData& Types::kHideInInspectorAttribute = Types::RegisterType("GameplayCore.Serialization", "HideInInspectorAttribute");
const TypeData& Types::kSerializeFieldAttribute = Types::RegisterType("GameplayCore.Serialization", "SerializeFieldAttribute");
const TypeData& Types::kInspectorNameAttribute = Types::RegisterType("GameplayCore.Editor", "InspectorNameAttribute");
const TypeData& Types::kSliderAttribute = Types::RegisterType("GameplayCore.Editor", "SliderAttribute");

const TypeData& Types::RegisterType(std::string name_space, std::string name) {
	TypeData type_data(name_space, name);

	assert(!registered_types_.contains(type_data) && "This type already registered.");

	auto result = registered_types_.insert(type_data);
	return *result.first;
}

const TypeData& Types::GetTypeData(std::string name_space, std::string name) {
	TypeData type_data(name_space, name);
	auto it = registered_types_.find(type_data);

	if (it != registered_types_.end()) {
		return *it;
	}

	return kUndefined;
}

const TypeData& Types::GetTypeData(std::string full_name) {
	std::string name_space;
	std::string name;
	Types::ParseFullName(full_name, name_space, name);
	return GetTypeData(name_space, name);
}

void Types::ParseFullName(const std::string& fullName, std::string& namespace_out, std::string& name_out)
{
	size_t lastDotPosition = fullName.find_last_of(".");
	namespace_out = fullName.substr(0, lastDotPosition);
	name_out = fullName.substr(lastDotPosition + 1, fullName.size() - lastDotPosition - 1);
}

} // namespace engine