#include "pch.h"
#include "CSharpObject.h"

#include <cassert>

namespace engine {

CSharpObject::CSharpObject(
	CSharpDomain& domain,
	CSharpAssembly& assembly,
	const std::string& namespace_name,
	const std::string& class_name) 
{
	// Get a reference to the class we want to instantiate
	class_ = GetClassInAssembly(assembly, namespace_name, class_name);

	// Allocate an instance of our class
	MonoObject* instance = mono_object_new(&domain.GetAppMonoDomain(), class_);
	assert(instance != nullptr); // TODO: exceptions and resources;

	// Call the parameterless (default) constructor
	mono_runtime_object_init(instance);
	object_ = instance;
}

CSharpObject::~CSharpObject() {
	delete object_;
}

MonoClass* CSharpObject::GetClassInAssembly(
	CSharpAssembly& assembly,
	const std::string& namespace_name, 
	const std::string& class_name) 
{
	MonoImage* image = mono_assembly_get_image(&assembly.GetMonoAssembly());
	MonoClass* clazz = mono_class_from_name(image, namespace_name.c_str(), class_name.c_str());

	if (clazz == nullptr) {
		// Log error here
		printf("Can't get class info!");
		return nullptr;
	}

	return clazz;
}

void CSharpObject::CallParameterlessMethod(const std::string& name)
{
	// Get a reference to the method in the class
	MonoMethod* method = mono_class_get_method_from_name(class_, name.c_str(), 0);
	assert(method != nullptr); // TODO: resources?

	// Call the C# method on the objectInstance instance, and get any potential exceptions
	MonoObject* exception = nullptr;
	mono_runtime_invoke(method, object_, nullptr, &exception);

	// TODO: Handle the exception
}

void CSharpObject::CallFloatMethod(const std::string& name, float value)
{
	// Get a reference to the method in the class
	MonoMethod* method = mono_class_get_method_from_name(class_, name.c_str(), 1);
	assert(method != nullptr); // TODO: resources?

	// Call the C# method on the objectInstance instance, and get any potential exceptions
	MonoObject* exception = nullptr;
	void* param = &value;
	mono_runtime_invoke(method, object_, &param, &exception);
}

} // namespace engine