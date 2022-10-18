#include "pch.h"
#include "CSharpType.h"
#include "CSharpException.h"

namespace engine {

MonoClass* CSharpType::GetMonoClass() const {
	return class_;
}

MonoType* CSharpType::GetMonoType() const {
	return type_;
}

CSharpType::CSharpType(MonoClass* clazz) 
	: class_(clazz)
{
	type_ = mono_class_get_type(class_);
}

CSharpType::CSharpType(
	const CSharpAssembly& assembly,
	const std::string& name_space,
	const std::string& name)
{
	class_ = mono_class_from_name(assembly.GetMonoImage(), name_space.c_str(), name.c_str());

	if (class_ == nullptr) {
		throw CSharpException("Could not get class" + name_space + name);
	}

	type_ = mono_class_get_type(class_);
}

} // namespace engine


