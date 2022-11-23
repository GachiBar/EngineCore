#include "mono_method_invoker.h"

namespace mono
{
mono_method_invoker::mono_method_invoker(mono_method method)
	: method_(std::move(method))
{}

auto mono::mono_method_invoker::invoke() -> std::optional<mono_object>
{
	return invoke(nullptr, nullptr);
}

auto mono::mono_method_invoker::invoke(void** args) -> std::optional<mono_object>
{
	return invoke(nullptr, args);
}

auto mono::mono_method_invoker::invoke(const mono_object& object) -> std::optional<mono_object>
{
	return invoke(object.get_internal_ptr(), nullptr);
}

auto mono::mono_method_invoker::invoke(const mono_object& object, void** args) -> std::optional<mono_object>
{
	return invoke(object.get_internal_ptr(), args);
}

auto mono_method_invoker::invoke(MonoObject* object, void** args) -> std::optional<mono_object>
{
	MonoObject* exception = nullptr;
	MonoObject* result = mono_runtime_invoke(method_.get_internal_ptr(), object, args, &exception);

	if (exception)
	{
		mono_print_unhandled_exception(exception);
		throw mono_thunk_exception(exception);
	}

	if (result != nullptr)
	{
		return mono_object(result);
	}
	
	return {};
}

} // namespace mono


