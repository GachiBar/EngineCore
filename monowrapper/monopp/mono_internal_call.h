#pragma once

#include "mono_config.h"

#include "mono_assembly.h"

BEGIN_MONO_INCLUDE
#include <mono/metadata/loader.h>
END_MONO_INCLUDE

namespace mono
{

template <typename F>
inline void add_internal_call(const std::string& name, F&& func)
{
	mono_add_internal_call(name.c_str(), reinterpret_cast<const void*>(func));
}

} // namespace mono
