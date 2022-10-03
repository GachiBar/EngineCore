#include "pch.h"
#include "CSharpDomain.h"

#include <cassert>
#include <vector>
#include <mono/metadata/assembly.h>

namespace engine {

MonoDomain& CSharpDomain::GetAppMonoDomain() {
	return *app_domain_;
}

CSharpDomain::CSharpDomain(
	const std::string& assemblies_path, 
	const std::string& script_runtime, 
	const std::string& app_domain)
{
	mono_set_assemblies_path("../mono/lib");

	MonoDomain* root_domain = mono_jit_init(script_runtime.c_str());
	assert(root_domain != nullptr);

	// Store the root domain pointer
	root_domain_ = root_domain;

	// Create an App Domain
	std::vector<char> cstr(app_domain.c_str(), app_domain.c_str() + app_domain.size() + 1);
	app_domain_ = mono_domain_create_appdomain(cstr.data(), nullptr);
	mono_domain_set(app_domain_, true);
}

CSharpDomain::~CSharpDomain() {
	delete root_domain_;
	delete app_domain_;
}

} // namespace engine