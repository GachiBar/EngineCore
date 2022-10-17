#include "mono_exception.h"
#include "mono_object.h"
#include "mono_property_invoker.h"
#include "mono_type.h"
#include "mono_string.h"

namespace mono
{

auto get_exception_info(MonoObject* ex) -> mono_thunk_exception::mono_exception_info
{
	auto obj = mono_object(ex);
	const auto& type = obj.get_type();
	const auto& exception_typename = type.get_fullname();

	auto message_prop = type.get_property("Message");
	mono_property_invoker mutable_msg_prop(message_prop);

	auto stacktrace_prop = type.get_property("StackTrace");
	mono_property_invoker mutable_stack_prop(stacktrace_prop);

	auto message_mono_obj = mutable_msg_prop.get_value(obj);
	mono_object message_obj(message_mono_obj);
	mono_string message(message_obj);

	auto stacktrace_mono_obj = mutable_stack_prop.get_value(obj);
	mono_object stacktrace_obj(stacktrace_mono_obj);
	mono_string stacktrace(stacktrace_obj);

	return {exception_typename, message.as_utf8(), stacktrace.as_utf8()};
}

mono_thunk_exception::mono_thunk_exception(MonoObject* ex)
	: mono_thunk_exception(get_exception_info(ex))
{
}

auto mono_thunk_exception::exception_typename() const -> const std::string&
{
	return info_.exception_typename;
}

auto mono_thunk_exception::message() const -> const std::string&
{
	return info_.message;
}

auto mono_thunk_exception::stacktrace() const -> const std::string&
{
	return info_.stacktrace;
}

mono_thunk_exception::mono_thunk_exception(const mono_exception_info& info)
	: mono_exception(info.exception_typename + "(" + info.message + ")\n" + info.stacktrace)
	, info_(info)
{
}

} // namespace mono
