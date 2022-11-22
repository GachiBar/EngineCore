#include "pch.h"
#include "Field.h"
#include "Object.h"

namespace engine {

const mono::mono_field& Field::GetInternal() const {
	return field_;
}

const std::string& Field::GetName() const {
	return field_.get_name();
}

const TypeData& Field::GetTypeData() const {
	return type_data_;
}

std::vector<Object> Field::GetAttributes() const {
	std::vector<Object> attributes;
	auto component_class = object_.GetInternal().get_type().get_internal_ptr();
	auto field = field_.get_internal_ptr();
	auto ainfo = mono_custom_attrs_from_field(component_class, field);

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

bool Field::IsPublic() const {
	return field_.get_visibility() == mono::visibility::vis_public;
}

Field::Field(const Object& object, const std::string& field_name)
	: Field(object, object.GetInternal().get_type().get_field(field_name))
{}

Field::Field(const Object& object, mono::mono_field field)
	: object_(object)
	, field_(std::move(field))
	, field_invoker_(field_)
	, type_data_(Types::GetTypeData(field_.get_type().get_fullname()))
{}

std::optional<Object> Field::GetValue() {
	auto raw_value = field_invoker_.get_value(object_.GetInternal());

	if (raw_value.has_value()) {
		mono::mono_object value(raw_value.value());
		return { value };
	}

	return {};
}

void Field::SetValue(void* data) {
	field_invoker_.set_value(object_.GetInternal(), data);
}

void Field::SetValue(const mono::mono_object& value) {
	SetValue(value.get_internal_ptr());
}

void Field::SetValue(const Object& value) {
	SetValue(value.GetInternal());
}

} // namespace engine;