#include "mono_method_invoker.h"

namespace mono
{
mono_method_invoker::mono_method_invoker(mono_method method)
	: method_(std::move(method))
{}

MonoObject* mono::mono_method_invoker::invoke()
{
	return invoke(nullptr, nullptr);
}

MonoObject* mono::mono_method_invoker::invoke(void** args)
{
	return invoke(nullptr, args);
}

MonoObject* mono::mono_method_invoker::invoke(const mono_object& object)
{
	return invoke(object.get_internal_ptr(), nullptr);
}

MonoObject* mono::mono_method_invoker::invoke(const mono_object& object, void** args)
{
	return invoke(object.get_internal_ptr(), args);
}

MonoObject* mono_method_invoker::operator()()
{
	return invoke();
}

MonoObject* mono_method_invoker::operator()(void** args)
{
	return invoke(args);
}

MonoObject* mono_method_invoker::operator()(const mono_object& object)
{
	return invoke(object);
}

MonoObject* mono_method_invoker::operator()(const mono_object& object, void** args)
{
	return invoke(object, args);
}

MonoObject* mono_method_invoker::invoke(MonoObject* object, void** args)
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

} // namespace mono


