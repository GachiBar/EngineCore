#pragma once

#include "mono_config.h"

BEGIN_MONO_INCLUDE
#include <mono/metadata/object.h>
END_MONO_INCLUDE

namespace mono 
{

class mono_array 
{
public:
	explicit mono_array(MonoArray* array);

	auto get_length() const->size_t;

	auto get(int index) const->MonoObject*;

protected:
	size_t length_;
	
	non_owning_ptr<MonoArray> array_;
};

} // namespace mono