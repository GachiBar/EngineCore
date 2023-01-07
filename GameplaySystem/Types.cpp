#include "pch.h"
#include "Types.h"

#include <unordered_set>
#include <cassert>

namespace engine {

std::unordered_set<TypeData> registered_types_;

const TypeData Types::kUndefined = Types::RegisterType("", "");

// System
const TypeData Types::kObject = Types::RegisterType("System", "Object");
const TypeData Types::kSingle = Types::RegisterType("System", "Single");
const TypeData Types::kDouble = Types::RegisterType("System", "Double");
const TypeData Types::kBoolean = Types::RegisterType("System", "Boolean");
const TypeData Types::kSByte = Types::RegisterType("System", "SByte");
const TypeData Types::kInt16 = Types::RegisterType("System", "Int16");
const TypeData Types::kInt32 = Types::RegisterType("System", "Int32");
const TypeData Types::kInt64 = Types::RegisterType("System", "Int64");
const TypeData Types::kByte = Types::RegisterType("System", "Byte");
const TypeData Types::kUInt16 = Types::RegisterType("System", "UInt16");
const TypeData Types::kUInt32 = Types::RegisterType("System", "UInt32");
const TypeData Types::kUInt64 = Types::RegisterType("System", "UInt64");
const TypeData Types::kVector2 = Types::RegisterType("GameplayCore.Mathematics", "Vector2");
const TypeData Types::kVector3 = Types::RegisterType("GameplayCore.Mathematics", "Vector3");
const TypeData Types::kVector4 = Types::RegisterType("GameplayCore.Mathematics", "Vector4");
const TypeData Types::kColor3 = Types::RegisterType("GameplayCore.Mathematics", "Color3");
const TypeData Types::kColor4 = Types::RegisterType("GameplayCore.Mathematics", "Color4");
const TypeData Types::kString = Types::RegisterType("System", "String");

// GameplayCore
const TypeData Types::kScene = Types::RegisterType("GameplayCore", "Scene");
const TypeData Types::kGameObject = Types::RegisterType("GameplayCore", "GameObject");

// GameplayCore.Components
const TypeData Types::kComponent = Types::RegisterType("GameplayCore.Components", "Component");
const TypeData Types::kTransformComponent = Types::RegisterType("GameplayCore.Components", "TransformComponent");

// Attributes
const TypeData Types::kHideInInspectorAttribute = Types::RegisterType("GameplayCore.Serialization", "HideInInspectorAttribute");
const TypeData Types::kSerializeFieldAttribute = Types::RegisterType("GameplayCore.Serialization", "SerializeFieldAttribute");
const TypeData Types::kInspectorNameAttribute = Types::RegisterType("GameplayCore.Editor", "InspectorNameAttribute");
const TypeData Types::kSliderAttribute = Types::RegisterType("GameplayCore.Editor", "SliderAttribute");

//Api
const TypeData Types::kRenderApi = Types::RegisterType("GameplayCore.EngineApi", "RenderApi");
const TypeData Types::kPhysicsApi = Types::RegisterType("GameplayCore.EngineApi", "PhysicsApi");
const TypeData Types::kTime = Types::RegisterType("GameplayCore", "Time");
const TypeData Types::kScreen = Types::RegisterType("GameplayCore", "Screen");
const TypeData Types::kInput = Types::RegisterType("GameplayCore", "Input");

// Resource
const TypeData Types::kResource = Types::RegisterType("GameplayCore.Resources", "Resource");
	
// AI
const TypeData Types::kAIArbitrator = Types::RegisterType("GameplayCore.AI", "AIArbitrator");
const TypeData Types::kAIAction = Types::RegisterType("GameplayCore.AI", "AIAction");

TypeDeclaration::TypeDeclaration(std::string name_space, std::string name)
	: name_space(std::move(name_space))
	, name(std::move(name))
{}

bool operator==(const TypeData& lhs, const TypeData& rhs) {
	if (lhs.type_hash != rhs.type_hash) {
		return false;
	}
	else if (lhs.full_name != rhs.full_name) {
		return false;
	}

	return true;
}

bool operator!=(const TypeData& lhs, const TypeData& rhs) {
	return !(lhs == rhs);
}

TypeData::TypeData(std::string name_space, std::string name)
	: name_space(std::move(name_space))
	, name(std::move(name))
	, full_name(this->name_space + "." + this->name)
	, type_hash(std::hash<std::string>{}(full_name))
{}

TypeData::TypeData(TypeDeclaration type_declaration)
	: TypeData(type_declaration.name_space, type_declaration.name)
{}

const TypeData Types::RegisterType(std::string name_space, std::string name) {
	TypeData type_data(name_space, name);

	assert(!registered_types_.contains(type_data) && "This type already registered.");

	auto result = registered_types_.insert(type_data);
	return *result.first;
}

const TypeData Types::GetTypeData(std::string name_space, std::string name) {
	TypeData type_data(name_space, name);
	auto it = registered_types_.find(type_data);

	if (it != registered_types_.end()) {
		return *it;
	}

	return kUndefined;
}

const TypeData Types::GetTypeData(std::string full_name) {
	auto type_declaration = Types::ParseFullName(full_name);
	return GetTypeData(std::move(type_declaration));
}

const TypeData Types::GetTypeData(TypeDeclaration type_declaration) {
	return GetTypeData(type_declaration.name_space, type_declaration.name);
}

TypeDeclaration Types::ParseFullName(const std::string& full_name)
{
	size_t lastDotPosition = full_name.find_last_of(".");
	auto name_space = full_name.substr(0, lastDotPosition);
	auto name = full_name.substr(lastDotPosition + 1, full_name.size() - lastDotPosition - 1);
	return { name_space, name };
}

} // namespace engine