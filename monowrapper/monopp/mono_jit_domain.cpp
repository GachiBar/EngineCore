#include "mono_jit_domain.h"
#include "mono_jit.h"

mono::mono_jit_domain::mono_jit_domain(const std::string& mono_assembly_path, const std::string& domain, bool enable_debugging)
{
	if (!mono::init_with_mono_assembly_path(mono_assembly_path, "KtripRuntime", enable_debugging)) {
		throw std::runtime_error("Can't init Mono with assembly");
	}
}
