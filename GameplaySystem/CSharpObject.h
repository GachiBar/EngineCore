#pragma once

#include "CSharpDomain.h"
#include "CSharpAssembly.h"

#include <mono/jit/jit.h>

namespace engine {

class CSharpObject {
public:
	MonoClass* GetMonoClass();
	MonoObject* GetMonoObject();

	CSharpObject(
		CSharpDomain& domain, 
		CSharpAssembly& assembly, 
		const std::string& namespace_name, 
		const std::string& class_name);

	CSharpObject(
		CSharpDomain& domain,
		CSharpAssembly& assembly,
		MonoObject* mono_object);

	~CSharpObject();

	MonoObject* CallMethod(const std::string& name);
	MonoObject* CallMethod(const std::string& name, void** params, size_t count);

private:
	CSharpDomain& domain_;
	CSharpAssembly& assembly_;
    MonoClass* class_;
	MonoObject* object_;

	MonoClass* GetClassInAssembly(
		CSharpAssembly& assembly,
		const std::string& namespace_name,
		const std::string& class_name);
};

} // namespace engine