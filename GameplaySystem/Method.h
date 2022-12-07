#pragma once

#include "Type.h"
#include "../monowrapper/monopp/mono_method.h"
#include "../monowrapper/monopp/mono_method_invoker.h"

#include <optional>

namespace engine {

class Object;

class Method {
public:
	const mono::mono_method& GetInternal() const;

	bool IsPublic() const;

	Method(Type type, const std::string& method_name_with_args);
	Method(Type type, const std::string& method_name, int argc);
	Method(Type type, mono::mono_method method);

	std::optional<Object> Invoke();
	std::optional<Object> Invoke(void** args);
	std::optional<Object> Invoke(const Object& object);
	std::optional<Object> Invoke(const Object& object, void** args);

private:
	const Type& type_;
	const mono::mono_method method_;
	mono::mono_method_invoker method_invoker_;
};

} // namespace engine