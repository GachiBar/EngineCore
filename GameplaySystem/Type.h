#pragma once

#include "Types.h"
#include "../monowrapper/monopp/mono_type.h"

namespace engine {

class Object;
class Field;
class Method;
class Property;

class Type {
public:
	const mono::mono_type& GetInternal() const;
	const TypeData& GetTypeData() const;

	const bool HasBaseType() const;
	const Type GetBaseType() const;

	const std::string GetName() const;
	const std::string GetNameSpace() const;
	const std::string GetFullName() const;

	const bool IsAbstract() const;
	const bool IsDerivedFrom(const Type& type) const;
	const bool IsDerivedFrom(const TypeData& type_data) const;

	Field GetField(const std::string& name) const;
	std::vector<Field> GetFields() const;

	Property GetProperty(const std::string& name) const;
	std::vector<Property> GetProperties() const;

	bool HasMethod(const std::string& name) const;
	bool HasMethod(const std::string& name, int argc) const;

	Method GetMethod(const std::string& name) const;
	Method GetMethod(const std::string& name, int argc) const;
	std::vector<Method> GetMethods() const;

	Type(mono::mono_type type);

	bool Is(const TypeData& type_data) const;
	Object Instantiate() const;

private:
	mono::mono_type type_;
	TypeData type_data_;
};

} // namespace engine