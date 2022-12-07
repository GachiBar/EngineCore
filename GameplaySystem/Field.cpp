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

const Type& Field::GetType() const {
	return field_type_;
}

std::vector<Object> Field::GetAttributes() const {
	std::vector<Object> attributes;
	auto object_class = class_type_.GetInternal().get_internal_ptr();
	auto field = field_.get_internal_ptr();
	auto ainfo = mono_custom_attrs_from_field(object_class, field);

	if (ainfo != nullptr) {
		for (size_t i = 0; i < ainfo->num_attrs; ++i) {
			auto centry = &ainfo->attrs[i];
			auto attribute_class = mono_method_get_class(centry->ctor);
			auto attribute = mono_custom_attrs_get_attr(ainfo, attribute_class);
			attributes.emplace_back(mono::mono_object(attribute));
		}
	}

	mono_free(ainfo);
	return attributes;
}

bool Field::IsPublic() const {
	return field_.get_visibility() == mono::visibility::vis_public;
}

Field::Field(Type type, const std::string& field_name)
	: Field(type, type.GetInternal().get_field(field_name))
{}

Field::Field(Type type, mono::mono_field field)
	: field_(std::move(field))
	, field_invoker_(field_)
	, class_type_(std::move(type))
	, field_type_(field_.get_type())
{}

std::optional<Object> Field::GetValue() {
	auto raw_value = field_invoker_.get_value();

	if (raw_value.has_value()) {
		return { raw_value.value() };
	}

	return {};
}

std::optional<Object> Field::GetValue(const Object& object) {
	auto raw_value = field_invoker_.get_value(object.GetInternal());

	if (raw_value.has_value()) {
		return { raw_value.value() };
	}

	return {};
}

void Field::SetValue(void* data) {
	field_invoker_.set_value(data);
}

void Field::SetValue(const Object& value) {
	SetValue(value.GetInternal());
}

void Field::SetValue(const Object& object, void* data) {
	field_invoker_.set_value(object.GetInternal(), data);
}

void Field::SetValue(const Object& object, const Object& value) {
	SetValue(object, value.GetInternal().get_internal_ptr());
}

} // namespace engine;