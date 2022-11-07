#include "pch.h"
#include "Method.h"
#include "Object.h"
#include "../monowrapper/monopp/mono_method_invoker.h"

namespace engine {	

const mono::mono_method& Method::GetInternal() const 	{
	return method_;
}

bool Method::IsPublic() const {
	return method_.get_visibility() == mono::visibility::vis_public;
}

Method::Method(const Object& object, mono::mono_method method)
	: object_(object)
	, method_(method)
	, method_invoker_(method)
{}

std::optional<Object> Method::Invoke(void** args) {
	auto result = method_invoker_.invoke(object_.GetInternal(), args);
	
	if (result != nullptr) {
		return { result };
	}

	return {};
}

} // namespace engine