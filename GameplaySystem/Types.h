#pragma once

#include <string>

namespace engine {

struct Types;

struct TypeDeclaration {
public:
	const std::string name_space;
	const std::string name;

	TypeDeclaration(std::string name_space, std::string name);
};

class TypeData {
	friend Types;

public:
	const std::string& GetNameSpace() const {
		return name_space;
	}

	const std::string& GetName() const {
		return name;
	}

	const std::string& GetFullName() const {
		return full_name;
	}

	const size_t GetTypeHash() const {
		return type_hash;
	}

	friend bool operator==(const TypeData& lhs, const TypeData& rhs);

	friend bool operator!=(const TypeData& lhs, const TypeData& rhs);

	TypeData(std::string name_space, std::string name);
	TypeData(TypeDeclaration type_declaration);

private:
	std::string name_space;
	std::string name;
	std::string full_name;
	size_t type_hash;
};

struct Types {		
public:
	static const TypeData kUndefined;

	// System
	static const TypeData kObject;
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
	static const TypeData kScene;
	static const TypeData kGameObject;

	// GameplayCore.Components
	static const TypeData kComponent;
	static const TypeData kTransformComponent;

	// Attributes
	static const TypeData kHideInInspectorAttribute;
	static const TypeData kSerializeFieldAttribute;
	static const TypeData kInspectorNameAttribute;
	static const TypeData kSliderAttribute;

	// Api
	static const TypeData kRenderApi;
	static const TypeData kPhysicsApi;
	static const TypeData kTime;
	static const TypeData kScreen;
	static const TypeData kInput;

	static const TypeData RegisterType(std::string name_space, std::string name);
	static const TypeData GetTypeData(std::string name_space, std::string name);
	static const TypeData GetTypeData(std::string full_name);
	static const TypeData GetTypeData(TypeDeclaration type_declaration);
	static TypeDeclaration ParseFullName(const std::string& full_name);
};

} // namespace engine

namespace std {

	template<>
	struct hash<engine::TypeData>
	{
		size_t operator()(const engine::TypeData& type_data) const noexcept
		{
			return type_data.GetTypeHash();
		}
	};

} // namespace std