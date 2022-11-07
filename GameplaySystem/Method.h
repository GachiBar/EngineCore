#pragma once

#include "../monowrapper/monopp/mono_method.h"
#include "../monowrapper/monopp/mono_method_invoker.h"

#include <optional>

namespace engine {

class Object;

class Method {
public:
	const mono::mono_method& GetInternal() const;

	bool IsPublic() const;

	Method(const Object& object, mono::mono_method method);

	std::optional<Object> Invoke(void** args);

private:
	const Object& object_;
	const mono::mono_method method_;
	mono::mono_method_invoker method_invoker_;
};

} // namespace engine