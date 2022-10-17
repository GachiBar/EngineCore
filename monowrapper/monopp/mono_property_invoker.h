#pragma once
#include "mono_property.h"

#include "mono_method_invoker.h"

namespace mono
{

class mono_property_invoker
{
public:
	mono_property_invoker(mono_property property)
		: property_(property)
	{
	}

	void set_value(void* val) const 
	{		
		mono_method_invoker thunk(property_.get_set_method());
		thunk.invoke(&val);
	}

	void set_value(const mono_object& obj, void* val) const 
	{
		mono_method_invoker thunk(property_.get_set_method());
		thunk.invoke(obj, &val);
	}

	auto get_value() const -> MonoObject* 
	{
		mono_method_invoker thunk(property_.get_get_method());
		return thunk.invoke();
	}

	auto get_value(const mono_object& obj) const -> MonoObject* 
	{
		mono_method_invoker thunk(property_.get_get_method());
		return thunk.invoke(obj);
	}

private:
	mono_property property_;
};

} // namespace mono
