#include "pch.h"
#include "Runtime.h"
#include "Type.h"
#include "Scene.h"
#include "GameObject.h"
#include "Component.h"

namespace engine {

const char* Runtime::kRuntimeName = "KtripRuntime";
const char* Runtime::kDomainName = "KtripDomain";

const Runtime* runtime_;

void Runtime::SetCurrentRuntime(const Runtime& runtime) {
	runtime_ = &runtime;
}

const Runtime& Runtime::GetCurrentRuntime() {
	return *runtime_;
}

const mono::mono_domain& Runtime::GetDomain() const {
	return domain_;
}

const mono::mono_assembly& Runtime::GetAssembly() const {
	return assembly_;
}

std::vector<std::string> Runtime::DumpTypeNames() const {
	return assembly_.dump_type_names();
}

std::vector<std::string> Runtime::DumpNonAbstractSubclassesOf(const TypeData& type_data) const {
	std::vector<std::string> names;
	auto typeNames = DumpTypeNames();

	for (auto typeName : typeNames)
	{
		try
		{
			auto typeDeclarartion = engine::Types::ParseFullName(typeName);
			auto type = engine::Runtime::GetCurrentRuntime().GetType(typeDeclarartion);

			if (!type.IsAbstract() &&
				type.HasBaseType() &&
				type.IsDerivedFrom(type_data))
			{
				names.push_back(typeName);
			}
		}
		catch (mono::mono_exception& ex)
		{
			continue;
		}
	}

	return names;
}

Type Runtime::GetType(const std::string& name_space, const std::string& name) const {
	return { assembly_.get_type(name_space, name) };
}

Type Runtime::GetType(const std::string& full_name) const {
	return GetType(Types::ParseFullName(full_name));
}

Type Runtime::GetType(const TypeDeclaration& declaration) const {
	return GetType(declaration.name_space, declaration.name);
}

Type Runtime::GetType(const TypeData& type_data) const {
	return GetType(type_data.GetNameSpace(), type_data.GetName());
}

#ifndef  _DEBUG
#define KTRIP_MONO_DEBUG false
#else
#define KTRIP_MONO_DEBUG true
#endif

Runtime::Runtime(const std::string& mono_lib_path, const std::string& dll_path)
	: jit_domain_(mono_lib_path, kRuntimeName, KTRIP_MONO_DEBUG)
	, domain_(kDomainName)
	, assembly_(domain_.get_assembly(dll_path))
{
	mono::mono_domain::set_current_domain(domain_);
	Scene::CacheMethods(*this);
	GameObject::CacheMethods(*this);
	Component::CacheMethods(*this);
}

} // namespace engine