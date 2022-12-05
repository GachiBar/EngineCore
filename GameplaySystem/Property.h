#pragma once

#include "Types.h"
#include "Type.h"
#include "../monowrapper/monopp/mono_string.h"
#include "../monowrapper/monopp/mono_property.h"
#include "../monowrapper/monopp/mono_property_invoker.h"

#include <string>
#include <optional>

namespace engine {

class Object;
class Method;

class Property {
public:
	const mono::mono_property& GetInternal() const;
	const std::string& GetName() const;
	const Type& GetType() const;
	std::vector<Object> GetAttributes() const;			

	bool CanRead() const;
	bool CanWrite() const;

	Method GetGetMethod() const;
	Method GetSetMethod() const;

	Property(Type type, const std::string name);
	Property(Type type, mono::mono_property property);

	std::optional<Object> GetValue();
	std::optional<Object> GetValue(const Object& object);

	void SetValue(void* data);
	void SetValue(const Object& value);

	void SetValue(const Object& object, void* data);
	void SetValue(const Object& object, const Object& value);

private:
	const mono::mono_property property_;
	const mono::mono_property_invoker property_invoker_;
	Type class_type_;
	Type property_type_;
};

} // namespace engine

