#pragma once

#include "mono_config.h"

#include "mono_type.h"
#include "mono_visibility.h"

BEGIN_MONO_INCLUDE
#include <mono/metadata/object.h>
END_MONO_INCLUDE

namespace mono
{
class mono_object;

class mono_field
{
public:
	explicit mono_field(const mono_type& type, const std::string& name);

	auto get_name() const -> const std::string&;
	auto get_fullname() const -> const std::string&;
	auto get_full_declname() const -> const std::string&;
	auto get_type() const -> const mono_type&;
	auto get_visibility() const -> visibility;
	auto is_static() const -> bool;
	auto get_internal_ptr() const -> MonoClassField*;
	auto get_internal_vtable_ptr() const -> MonoVTable*;
	auto is_valuetype() const -> bool;

protected:
	void generate_meta();
	
	mono_type type_;
	non_owning_ptr<MonoClassField> field_ = nullptr;
	non_owning_ptr<MonoVTable> owning_type_vtable_ = nullptr;
	std::string name_;
	std::string fullname_;
	std::string full_declname_;
};

} // namespace mono
