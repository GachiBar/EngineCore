#pragma once

#include "../GameplaySystem/GameObject.h"

#include <vector>
#include "libs/Delegates.h"

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

	void draw_imgui(const engine::Scene& scene);

	Delegate<void, std::shared_ptr<engine::GameObject>&> OnSelectGameObjectInHierarchy;
};

