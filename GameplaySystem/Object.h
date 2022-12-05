#pragma once

#include "Type.h"
#include "Field.h"
#include "Property.h"
#include "Method.h"
#include "../monowrapper/monopp/mono_object.h"

namespace engine {

class Object {
 public:
	 const mono::mono_object& GetInternal() const;
	 const Type& GetType() const;

	 Object(MonoObject* mono_object);
	 Object(const mono::mono_object& object);
	 Object(const Object& other);

	 virtual ~Object();

	 template<typename T>
	 T Unbox() const;
	 void* Unbox() const;

	 friend bool operator== (const Object& lhs, const Object& rhs);
	 friend bool operator== (const Object& lhs, const mono::mono_object& rhs);
	 friend bool operator== (const mono::mono_object& lhs, const Object& rhs);

 private:
	mono::mono_object object_;
	Type type_;
	uint32_t handle_;	
};

template<typename T>
T Object::Unbox() const {
	return *reinterpret_cast<T*>(object_.unbox());
}

} // namespace engine