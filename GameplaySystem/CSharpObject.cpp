#include "pch.h"
#include "CSharpObject.h"

#include <cassert>

namespace engine {

MonoClass* CSharpObject::GetMonoClass() const {
	return class_;
}

MonoObject* CSharpObject::GetMonoObject() const {
	return object_;
}

CSharpObject::CSharpObject(
	CSharpDomain& domain,
	CSharpAssembly& assembly,
	const std::string& namespace_name,
	const std::string& class_name) 
	: domain_(domain)
	, assembly_(assembly)
{
	// Get a reference to the class we want to instantiate
	class_ = GetClassInAssembly(assembly_, namespace_name, class_name);

	// Allocate an instance of our class
	MonoObject* instance = mono_object_new(domain_.GetAppMonoDomain(), class_);
	assert(instance != nullptr); // TODO: exceptions and resources;

	// Call the parameterless (default) constructor
	mono_runtime_object_init(instance);
	object_ = instance;
}

CSharpObject::CSharpObject(
	CSharpDomain& domain, 
	CSharpAssembly& assembly, 
	MonoObject* mono_object)
	: domain_(domain)
	, assembly_(assembly)
{
	class_ = mono_object_get_class(mono_object);
	object_ = mono_object;
}

MonoObject* CSharpObject::CallMethod(const std::string& name) const {
	// Get a reference to the method in the class
	MonoMethod* method = mono_class_get_method_from_name(class_, name.c_str(), 0);
	assert(method != nullptr); // TODO: resources?

	// Call the C# method on the objectInstance instance, and get any potential exceptions
	MonoObject* exception = nullptr;
	MonoObject* result = mono_runtime_invoke(method, object_, nullptr, &exception);

	if (exception)
	{
		mono_print_unhandled_exception(exception);
	}

	return result;
}

MonoObject* CSharpObject::CallMethod(const std::string& name, void** params, size_t count) const {
	// Get a reference to the method in the class
	MonoMethod* method = mono_class_get_method_from_name(class_, name.c_str(), count);
	assert(method != nullptr); // TODO: resources?

	// Call the C# method on the objectInstance instance, and get any potential exceptions
	MonoObject* exception = nullptr;
	MonoObject* result = mono_runtime_invoke(method, object_, params, &exception);

	if (exception)
	{
		mono_print_unhandled_exception(exception);
	}

	return result;
}

MonoClass* CSharpObject::GetClassInAssembly(
	CSharpAssembly& assembly,
	const std::string& namespace_name, 
	const std::string& class_name) 
{
	MonoImage* image = mono_assembly_get_image(assembly.GetMonoAssembly());
	MonoClass* clazz = mono_class_from_name(image, namespace_name.c_str(), class_name.c_str());

	if (clazz == nullptr) {
		// Log error here
		printf("Can't get class info!");
		return nullptr;
	}

	return clazz;
}

} // namespace engine