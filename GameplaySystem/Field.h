#pragma once

#include "Types.h"
#include "../monowrapper/monopp/mono_field.h"
#include "../monowrapper/monopp/mono_field_invoker.h"

#include <optional>

namespace engine {

class Object;

class Field {
public:
	const mono::mono_field& GetInternal() const;
	const std::string& GetName() const;
	const TypeData& GetTypeData() const;
	std::vector<Object> GetAttributes() const;

	bool IsPublic() const;
	
	Field(const Object& object, const std::string& field_name);
	Field(const Object& object, mono::mono_field field);

	std::optional<Object> GetValue();

	void SetValue(void* data);
	void SetValue(const mono::mono_object& value);
	void SetValue(const Object& value);

private:
	const Object& object_;
	const mono::mono_field field_;
	const mono::mono_field_invoker field_invoker_;
	const TypeData& type_data_;
};

} // namespace engine