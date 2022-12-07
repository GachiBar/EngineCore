#pragma once

#include "Type.h"
#include "../monowrapper/monopp/mono_field.h"
#include "../monowrapper/monopp/mono_field_invoker.h"

#include <optional>

namespace engine {

class Object;

class Field {
public:
	const mono::mono_field& GetInternal() const;
	const std::string& GetName() const;
	const Type& GetType() const;
	std::vector<Object> GetAttributes() const;

	bool IsPublic() const;
	
	Field(Type object, const std::string& field_name);
	Field(Type object, mono::mono_field field);

	std::optional<Object> GetValue();
	std::optional<Object> GetValue(const Object& object);

	void SetValue(void* data);
	void SetValue(const Object& value);

	void SetValue(const Object& object, void* data);
	void SetValue(const Object& object, const Object& value);

private:
	const mono::mono_field field_;
	const mono::mono_field_invoker field_invoker_;
	Type class_type_;
	Type field_type_;
};

} // namespace engine