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
	mono_method_invoker(mono_method method);

	MonoObject* invoke();

	MonoObject* invoke(void** args);

	MonoObject* invoke(const mono_object& object);

	MonoObject* invoke(const mono_object& object, void** args);

	MonoObject* operator()();

	MonoObject* operator()(void** args);

	MonoObject* operator()(const mono_object& object);

	MonoObject* operator()(const mono_object& object, void** args);

private:
	mono_method method_;

	MonoObject* invoke(MonoObject* object, void** args);
};

} // namespace mono
