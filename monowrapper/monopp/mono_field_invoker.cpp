#include "mono_field_invoker.h"

namespace mono
{

mono_field_invoker::mono_field_invoker(mono_field field)
	: field_(std::move(field))
{}

void mono_field_invoker::set_value(void* val) const
{
	mono_field_static_set_value(field_.get_internal_vtable_ptr(), field_.get_internal_ptr(), val);
}

void mono_field_invoker::set_value(const mono_object& obj, void* val) const
{
	mono_field_set_value(obj.get_internal_ptr(), field_.get_internal_ptr(), val);
}

auto mono_field_invoker::get_value() const -> std::optional<mono_object>
{
	MonoObject* val = nullptr;
	mono_field_static_get_value(field_.get_internal_vtable_ptr(), field_.get_internal_ptr(), val);

	if (val != nullptr) 
	{
		return mono_object(val);
	}

	return {};
}

auto mono_field_invoker::get_value(const mono_object& obj) const -> std::optional<mono_object>
{
	auto& domain = mono::mono_domain::get_current_domain();
	MonoObject* result = mono_field_get_value_object(domain.get_internal_ptr(), field_.get_internal_ptr(), obj.get_internal_ptr());

	if (result != nullptr)
	{
		return mono_object(result);
	}

	return {};
}

} // namespace mono

