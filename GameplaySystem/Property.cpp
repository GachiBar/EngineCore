#include "pch.h"
#include "Property.h"
#include "Attribute.h"
#include "Types.h"

#include <mono/metadata/object.h>

namespace engine {

const mono::mono_property& Property::GetInternal() const {
	return property_;
}

std::vector<Attribute> Property::GetAttributes() const {
	std::vector<Attribute> attributes;
	auto component_class = object_.GetInternal().get_type().get_internal_ptr();
	auto property = property_.get_internal_ptr();
	auto ainfo = mono_custom_attrs_from_property(component_class, property);

	if (ainfo != nullptr) {
		for (size_t i = 0; i < ainfo->num_attrs; ++i) {
			auto centry = &ainfo->attrs[i];
			auto attribute_class = mono_method_get_class(centry->ctor);
			auto attribute = mono_custom_attrs_get_attr(ainfo, attribute_class);
			attributes.emplace_back(mono::mono_object(attribute));
		}
	}

	return attributes;
}

PropertyType Property::GetType() const {
	return type_;
}

std::string Property::GetName() const {
	return property_.get_name();
}

bool Property::CanRead() const {
	auto getter = mono_property_get_get_method(property_.get_internal_ptr());
	return getter != nullptr;
}

bool Property::CanWrite() const {
	auto setter = mono_property_get_set_method(property_.get_internal_ptr());
	return setter != nullptr;
}

Method Property::GetGetMethod() const {
	return { object_, property_.get_get_method() };
}

Method Property::GetSetMethod() const {
	return { object_, property_.get_set_method() };
}

std::optional<Object> Property::GetValue() {
	auto raw_value = property_invoker_.get_value(object_.GetInternal());

	if (raw_value != nullptr) {
		mono::mono_object value(property_invoker_.get_value(object_.GetInternal()));
		return { value };
	}

	return {};
}

void Property::SetValue(void* data) {
	property_invoker_.set_value(object_.GetInternal(), data);
}

void Property::SetValue(const mono::mono_object& value) {
	SetValue(value.get_internal_ptr());
}

void Property::SetValue(const Object& value) {
	SetValue(value.GetInternal());
}

Property::Property(const Object& object, const std::string& property_name)
	: Property(object, object.GetInternal().get_type().get_property(property_name))
{}

Property::Property(const Object& object, mono::mono_property property)
	: object_(object)
	, property_(std::move(property))
	, property_invoker_(property_)
	, type_(NameToPropertyType(property_.get_type().get_fullname()))
{}

PropertyType NameToPropertyType(const std::string& name) {
	if (name == Types::kSingle.full_name) {
		return PropertyType::kFloat;
	}		
	if (name == Types::kDouble.full_name) {
		return PropertyType::kDouble;
	}		
	if (name == Types::kBoolean.full_name) {
		return PropertyType::kBool;
	}		
	if (name == Types::kSByte.full_name) {
		return PropertyType::kByte;
	}		
	if (name == Types::kInt16.full_name) {
		return PropertyType::kShort;
	}		
	if (name == Types::kInt32.full_name) {
		return PropertyType::kInt;
	}		
	if (name == Types::kInt64.full_name) {
		return PropertyType::kLong;
	}		
	if (name == Types::kByte.full_name) {
		return PropertyType::kUByte;
	}		
	if (name == Types::kUInt16.full_name) {
		return PropertyType::kUShort;
	}		
	if (name == Types::kUInt32.full_name) {
		return PropertyType::kUInt;
	}		
	if (name == Types::kUInt64.full_name) {
		return PropertyType::kULong;
	}		
	if (name == Types::kVector2.full_name) {
		return PropertyType::kVector2;
	}		
	if (name == Types::kVector3.full_name) {
		return PropertyType::kVector3;
	}		
	if (name == Types::kVector4.full_name) {
		return PropertyType::kVector4;
	}		
	if (name == Types::kString.full_name) {
		return PropertyType::kString;
	}		
	if (name == Types::kGameObject.full_name) {
		return PropertyType::kGameObject;
	}		

	return PropertyType::kUndefined;
}

} // namespace engine
