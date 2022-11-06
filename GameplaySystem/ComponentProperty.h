#pragma once

#include "../monowrapper/monopp/mono_string.h"
#include "../monowrapper/monopp/mono_property_invoker.h"

#include <string>
#include <optional>

namespace engine {

class Component;

enum class PropertyType {
	kUndefined,
	kFloat, 
	kDouble,
	kBool,
	kByte,
	kShort,
	kInt,
	kLong,
	kUByte,
	kUShort,
	kUInt,
	kULong,
	kVector2,
	kVector3,
	kVector4,
	kString,
	kGameObject,
};

class ComponentProperty {

	friend class Component;

public:
	const mono::mono_property& GetInternal() const;
	std::vector<mono::mono_object> GetAttributes() const;
	PropertyType GetType() const;

	std::string GetName() const;

	std::optional<mono::mono_object> GetValue();

	void SetValue(void* data);
	void SetValue(const mono::mono_object& value);
	
private:
	const Component& component_;
	const mono::mono_property property_;
	const mono::mono_property_invoker property_invoker_;
	const PropertyType type_;

	ComponentProperty(const Component& component, std::string name);
	ComponentProperty(const Component& component, mono::mono_property property);
};

PropertyType NameToPropertyType(const std::string& name);

} // namespace engine

