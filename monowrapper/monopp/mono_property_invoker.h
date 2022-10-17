#pragma once
#include "mono_property.h"

#include "mono_method_invoker.h"

namespace mono
{

class mono_property_invoker
{
public:
	mono_property_invoker(mono_property property);

	void set_value(void* val) const;
	void set_value(const mono_object& obj, void* val) const;
	auto get_value() const->MonoObject*;
	auto get_value(const mono_object& obj) const->MonoObject*;

private:
	mono_property property_;
};

} // namespace mono
