#pragma once

#include "Types.h"
#include "Field.h"
#include "Property.h"
#include "Method.h"
#include "../monowrapper/monopp/mono_object.h"

namespace engine {

class Object {
 public:
	 const mono::mono_object& GetInternal() const;
	 const mono::mono_type& GetType() const;
	 const mono::mono_type& GetBaseType() const;
	 const TypeData& GetTypeData() const;

	 const std::string& GetNameSpace() const;
	 const std::string& GetName() const;
	 const std::string& GetFullName() const;

	 Property GetProperty(const std::string& name) const;
	 std::vector<Property> GetProperties() const;

	 Field GetField(const std::string& name) const;
	 std::vector<Field> GetFields() const;

	 bool HasMethod(const std::string& name) const;
	 bool HasMethod(const std::string& name, int argc) const;

	 Method GetMethod(const std::string& name) const;
	 Method GetMethod(const std::string& name, int argc) const;
	 std::vector<Method> GetMethods() const;

	 Object(MonoObject* mono_object);
	 Object(mono::mono_object object);
	 Object(MonoObject* mono_object, MonoClass* mono_class);
	 Object(mono::mono_object object, mono::mono_type type);
	 virtual ~Object();

	 Object ToBase() const;
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
	mono::mono_type type_;
	uint32_t handle_;	
	const TypeData& type_data_;
};

} // namespace engine