#pragma once

#include "mono_config.h"
#include "mono_object.h"

BEGIN_MONO_INCLUDE
#include <mono/metadata/object.h>
END_MONO_INCLUDE

namespace mono 
{

class mono_array 
{
public:
	explicit mono_array(MonoArray* array);

	auto get_length() const -> size_t;

	template<typename T>
	auto get(int index) const -> T
	{
		return mono_array_get(array_, T, index);
	}

protected:
	size_t length_;
	
	non_owning_ptr<MonoArray> array_;
};

} // namespace mono