#pragma once

#include "CSharpDomain.h"
#include "CSharpAssembly.h"

#include <mono/jit/jit.h>

namespace engine {

class CSharpObject {
public:
	CSharpObject(
		CSharpDomain& domain, 
		CSharpAssembly& assembly, 
		const std::string& namespace_name, 
		const std::string& class_name);

	~CSharpObject();

	void CallParameterlessMethod(const std::string& name);
	void CallFloatMethod(const std::string& name, float value);

private:
    MonoClass* class_;
	MonoObject* object_;

	MonoClass* GetClassInAssembly(
		CSharpAssembly& assembly,
		const std::string& namespace_name,
		const std::string& class_name);
};

} // namespace engine