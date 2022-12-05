#include "pch.h"
#include "Runtime.h"
#include "Type.h"
#include "Scene.h"
#include "GameObject.h"
#include "Component.h"

namespace engine {

const char* Runtime::kRuntimeName = "KtripRuntime";
const char* Runtime::kDomainName = "KtripDomain";

const mono::mono_domain& Runtime::GetDomain() const {
	return domain_;
}

const mono::mono_assembly& Runtime::GetAssembly() const {
	return assembly_;
}

Type Runtime::GetType(const std::string& name_space, const std::string& name) const {
	return { assembly_.get_type(name_space, name) };
}

Type Runtime::GetType(const TypeData& type_data) const {
	return GetType(type_data.name_space, type_data.name);
}

Runtime::Runtime(const std::string& mono_lib_path, const std::string& dll_path)
	: jit_domain_(mono_lib_path, kRuntimeName)
	, domain_(kDomainName)
	, assembly_(domain_.get_assembly(dll_path))
{
	mono::mono_domain::set_current_domain(domain_);
	Scene::CacheMethods(*this);
	GameObject::CacheMethods(*this);
	Component::CacheMethods(*this);
}

} // namespace engine