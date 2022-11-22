#pragma once

#include "mono_config.h"

#include "mono_exception.h"
#include "mono_method.h"
#include "mono_object.h"
#include "mono_type.h"

#include <optional>

namespace mono
{

class mono_method_invoker 
{
public:
	mono_method_invoker(mono_method method);

	auto invoke() -> std::optional<mono_object>;
	auto invoke(void** args) -> std::optional<mono_object>;
	auto invoke(const mono_object& object) -> std::optional<mono_object>;
	auto invoke(const mono_object& object, void** args) -> std::optional<mono_object>;

private:
	mono_method method_;

	auto invoke(MonoObject* object, void** args) -> std::optional<mono_object>;
};

} // namespace mono
