#include "mono_array.h"

mono::mono_array::mono_array(MonoArray* array)
	: array_(array)
	, length_(mono_array_length(array))
{}

auto mono::mono_array::get_length() const -> size_t
{
	return length_;
}

auto mono::mono_array::get(int index) const -> MonoObject*
{
	return mono_array_get(array_, MonoObject*, index);
}