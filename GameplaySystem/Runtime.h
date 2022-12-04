#pragma once

#include "../monowrapper/monopp/mono_jit_domain.h"
#include "../monowrapper/monopp/mono_domain.h"
#include "../monowrapper/monopp/mono_assembly.h"

namespace engine {

class Type;

class Runtime {
public:	
	const mono::mono_domain& GetDomain() const;
	const mono::mono_assembly& GetAssembly() const;
	Type GetType(const std::string& name_space, const std::string& name) const;

	Runtime(const std::string& mono_lib_path, const std::string& dll_path);

private:
	static const char* kRuntimeName;
	static const char* kDomainName;

	mono::mono_jit_domain jit_domain_;
	mono::mono_domain domain_;
	mono::mono_assembly assembly_;
};

} // namespace engine