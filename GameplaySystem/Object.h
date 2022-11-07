#pragma once

#include "Property.h"
#include "Method.h"
#include "../monowrapper/monopp/mono_object.h"

namespace engine {

class Object {
 public:
	 const mono::mono_object& GetInternal() const;

	 Property GetProperty(const std::string& name) const;
	 std::vector<Property> GetProperties() const;

	 Method GetMethod(const std::string& name) const;
	 Method GetMethod(const std::string& name, int argc) const;
	 std::vector<Method> GetMethods() const;

	 Object(MonoObject* mono_object);
	 Object(mono::mono_object object);
	 virtual ~Object();

	 void* Unbox() const;

	 template<typename T>
	 T Unbox() const {
		 return *(T*)object_.unbox();
	 }

	 friend bool operator== (const Object& lhs, const Object& rhs);
	 friend bool operator== (const Object& lhs, const mono::mono_object& rhs);
	 friend bool operator== (const mono::mono_object& lhs, const Object& rhs);

 private:
	mono::mono_object object_;
	uint32_t handle_;	
};

} // namespace engine