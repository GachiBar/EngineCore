#pragma once

#include "mono_config.h"

namespace mono
{

// TODO: May be we should rewrite original mono_jit and incapsulate mono_jit logic in this class.
class mono_jit_domain {
public:
	explicit mono_jit_domain(const std::string& mono_assembly_path, const std::string& domain, bool enable_debugging = false);
	~mono_jit_domain();
};

} // namespace mono