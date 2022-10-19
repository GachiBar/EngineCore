#pragma once
#include <memory>

namespace mono
{
	class mono_object;
}

class PropertyWindow
{
public:
	void draw_imgui(std::shared_ptr<mono::mono_object> go);
};
