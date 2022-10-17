#pragma once

#include "mono_config.h"

#include "mono_exception.h"
#include "mono_method.h"
#include "mono_object.h"
#include "mono_type.h"

namespace mono
{

class mono_method_invoker 
{
public:
	mono_method_invoker(mono_method method)
		: method_(std::move(method))
	{}

	MonoObject* invoke()
	{
		return invoke(nullptr, nullptr);
	}

	MonoObject* invoke(void** args)
	{
		return invoke(nullptr, args);
	}

	MonoObject* invoke(const mono_object& object)
	{
		return invoke(object.get_internal_ptr(), nullptr);
	}

	MonoObject* invoke(const mono_object& object, void** args) 
	{
		return invoke(object.get_internal_ptr(), args);
	}

private:
	mono_method method_;

	MonoObject* invoke(MonoObject* object, void** args) 
	{
		MonoObject* exception = nullptr;
		MonoObject* result = mono_runtime_invoke(method_.get_internal_ptr(), object, args, &exception);

		if (exception)
		{
			mono_print_unhandled_exception(exception);
			throw mono_thunk_exception(exception);
		}

		return result;
	}
};

} // namespace mono
