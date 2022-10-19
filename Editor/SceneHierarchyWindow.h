#pragma once
#include <string>
#include <vector>
#include "Delegates.h"

namespace mono
{
	class mono_object;
}

namespace engine
{
	class CSharpObject;
}

class SceneHierarchyWindow
{
public:

	void draw_imgui(mono::mono_object& scene);

	Delegate<void, std::shared_ptr<mono::mono_object>& > OnSelectGameObjectInHierarchy;

	std::vector< std::shared_ptr<mono::mono_object>> GetGameObjects(mono::mono_object& scene) const;
};

