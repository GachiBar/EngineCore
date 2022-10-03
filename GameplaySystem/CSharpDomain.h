#pragma once

#include <string>
#include <mono/jit/jit.h>

namespace engine {

class CSharpDomain {
public:

	MonoDomain& GetAppMonoDomain();

	CSharpDomain(
		const std::string& assemblies_path,
		const std::string& script_runtime,
		const std::string& app_domain);

	~CSharpDomain();

private:
	MonoDomain* root_domain_;
	MonoDomain* app_domain_;
};

} // namespace engine