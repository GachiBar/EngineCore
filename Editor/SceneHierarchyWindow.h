#pragma once

#include "../GameplaySystem/Scene.h"
#include "../GameplaySystem/GameObject.h"
#include "libs/Delegates.h"

#include <vector>

class SceneHierarchyWindow
{
public:
	std::shared_ptr<engine::Scene> GetScene();
	void SetScene(std::shared_ptr<engine::Scene> scene);

	SceneHierarchyWindow(const mono::mono_assembly& assembly);

	void draw_imgui();

	DECLARE_EVENT(GameObjectSelectedEvent, SceneHierarchyWindow, std::shared_ptr<engine::GameObject>)
	GameObjectSelectedEvent GameObjectSelected;

private:
	std::shared_ptr<engine::Scene> scene;
	const mono::mono_assembly& assembly;

	void BuildTree(engine::Component& transform, int id);

	static bool SelectableTreeNode(const char* label, bool* isSelected);
};

