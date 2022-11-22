#include "mono_property_invoker.h"

namespace mono
{

mono::mono_property_invoker::mono_property_invoker(mono_property property)
	: property_(property)
{
}

void mono::mono_property_invoker::set_value(void* val) const
{
	mono_method_invoker thunk(property_.get_set_method());
	thunk.invoke(&val);
}

void mono::mono_property_invoker::set_value(const mono_object& obj, void* val) const
{
	mono_method_invoker thunk(property_.get_set_method());
	thunk.invoke(obj, &val);
}

auto mono::mono_property_invoker::get_value() const -> std::optional<mono_object>
{
	mono_method_invoker thunk(property_.get_get_method());
	return thunk.invoke();
}

auto mono::mono_property_invoker::get_value(const mono_object& obj) const -> std::optional<mono_object>
{
	mono_method_invoker thunk(property_.get_get_method());
	return thunk.invoke(obj);
}

} // namespace mono


