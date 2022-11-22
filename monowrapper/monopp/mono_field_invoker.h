#pragma once
#include "mono_field.h"

#include "mono_domain.h"
#include "mono_object.h"

#include <optional>

namespace mono
{

class mono_field_invoker
{
public:
	mono_field_invoker(mono_field field);

	void set_value(void* val) const;
	void set_value(const mono_object& obj, void* val) const;
	auto get_value() const -> std::optional<mono_object>;
	auto get_value(const mono_object& obj) const -> std::optional<mono_object>;

private:
	mono_field field_;
};

} // namespace mono
