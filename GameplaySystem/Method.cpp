#include "pch.h"
#include "Method.h"
#include "Object.h"
#include "../monowrapper/monopp/mono_method_invoker.h"

#include <cassert>

namespace engine {	

const mono::mono_method& Method::GetInternal() const 	{
	return method_;
}

bool Method::IsPublic() const {
	return method_.get_visibility() == mono::visibility::vis_public;
}

Method::Method(const Type& type, const std::string& method_name_with_args)
	: Method(type, type.GetInternal().get_method(method_name_with_args))
{}

Method::Method(const Type& type, const std::string& method_name, int argc)
	: Method(type, type.GetInternal().get_method(method_name, argc))
{}

Method::Method(const Type& type, mono::mono_method method)
	: type_(type)
	, method_(std::move(method))
	, method_invoker_(method_)
{
	assert(!method_.is_static() && "Method should not be static.");
}

std::optional<Object> Method::Invoke() {
	return Invoke(nullptr);
}

std::optional<Object> Method::Invoke(void** args) {
	auto result = method_invoker_.invoke(args);
	
	if (result.has_value()) {
		return { result };
	}

	return {};
}

std::optional<Object> Method::Invoke(const Object& object) {
	return Invoke(object.GetInternal(), nullptr);
}

std::optional<Object> Method::Invoke(const Object& object, void** args) {
	auto result = method_invoker_.invoke(object.GetInternal(), args);

	if (result.has_value()) {
		return { result };
	}

	return {};
}

} // namespace engine