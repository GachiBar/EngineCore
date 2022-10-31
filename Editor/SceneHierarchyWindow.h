#pragma once

#include "../GameplaySystem/GameObject.h"
#include "../GameplaySystem/Scene.h"

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

	void draw_imgui(const engine::Scene& scene);

	Delegate<void, std::shared_ptr<engine::GameObject>&> OnSelectGameObjectInHierarchy;
};

