#pragma once

#include <string>

namespace engine {

struct Types;

struct TypeData {
	friend Types;

public:
	const std::string name_space;
	const std::string name;
	const std::string full_name;
	const size_t type_hash;

	friend bool operator==(const TypeData& lhs, const TypeData& rhs) {
		if (lhs.type_hash != rhs.type_hash) {
			return false;
		} else if (lhs.full_name != rhs.full_name) {
			return false;
		}

		return true;
	}

	friend bool operator!=(const TypeData& lhs, const TypeData& rhs) {
		return !(lhs == rhs);
	}

private:
	TypeData(std::string name_space, std::string name)
		: name_space(name_space)
		, name(name)
		, full_name(name_space + "." + name)
		, type_hash(std::hash<std::string>{}(full_name))
	{}
};

struct Types {		
public:
	static const TypeData& kUndefined;

	// System
	static const TypeData& kObject;
	static const TypeData& kSingle;
	static const TypeData& kDouble;
	static const TypeData& kBoolean;
	static const TypeData& kSByte;
	static const TypeData& kInt16;
	static const TypeData& kInt32;
	static const TypeData& kInt64;
	static const TypeData& kByte;
	static const TypeData& kUInt16;
	static const TypeData& kUInt32;
	static const TypeData& kUInt64;
	static const TypeData& kVector2;
	static const TypeData& kVector3;
	static const TypeData& kVector4;
	static const TypeData& kString;

	// GameplayCore
	static const TypeData& kGameObject;

	// GameplayCore.Components
	static const TypeData& kComponent;
	static const TypeData& kTransformComponent;

	// Attributes
	static const TypeData& kHideInInspectorAttribute;
	static const TypeData& kSerializeFieldAttribute;
	static const TypeData& kInspectorNameAttribute;
	static const TypeData& kSliderAttribute;

	static const TypeData& RegisterType(std::string name_space, std::string name);
	static const TypeData& GetTypeData(std::string name_space, std::string name);
	static const TypeData& GetTypeData(std::string full_name);
	static void ParseFullName(const std::string& fullName, std::string& namespace_out, std::string& name_out);
};

} // namespace engine

namespace std {

	template<>
	struct hash<engine::TypeData>
	{
		size_t operator()(const engine::TypeData& type_data) const noexcept
		{
			return type_data.type_hash;
		}
	};

} // namespace std