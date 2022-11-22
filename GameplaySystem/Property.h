#pragma once

#include "Types.h"
#include "../monowrapper/monopp/mono_string.h"
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
	const TypeData& GetTypeData() const;
	std::vector<Object> GetAttributes() const;			

	bool CanRead() const;
	bool CanWrite() const;

	Method GetGetMethod() const;
	Method GetSetMethod() const;

	Property(const Object& object, const std::string& property_name);
	Property(const Object& object, mono::mono_property property);

	std::optional<Object> GetValue();

	void SetValue(void* data);
	void SetValue(const mono::mono_object& value);
	void SetValue(const Object& value);

private:
	const Object& object_;
	const mono::mono_property property_;
	const mono::mono_property_invoker property_invoker_;
	const TypeData& type_data_;
};

} // namespace engine

