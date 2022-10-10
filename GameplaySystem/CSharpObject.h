#pragma once

#include "CSharpDomain.h"
#include "CSharpAssembly.h"

#include <mono/jit/jit.h>

namespace engine {

class CSharpObject {
public:
	MonoClass* GetMonoClass() const;
	MonoObject* GetMonoObject() const;

	CSharpObject(
		CSharpDomain& domain, 
		CSharpAssembly& assembly, 
		const std::string& namespace_name, 
		const std::string& class_name);

	CSharpObject(
		CSharpDomain& domain,
		CSharpAssembly& assembly,
		MonoObject* mono_object);

	MonoObject* CallMethod(const std::string& name) const;
	MonoObject* CallMethod(const std::string& name, void** params, size_t count) const;

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