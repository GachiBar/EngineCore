#pragma once

#include "../GameplaySystem/Scene.h"
#include "../GameplaySystem/GameObject.h"
#include "libs/Delegates.h"

#include <vector>
#include <functional>

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
	int tree_level_id_;

	std::shared_ptr<engine::Scene> scene;
	std::shared_ptr<engine::GameObject> selected;
	const mono::mono_assembly& assembly;

	void BuildTree(engine::Component& transform);
	void DrawPopup(std::shared_ptr<engine::GameObject> gameObject);

	bool IsSelected(std::shared_ptr<engine::GameObject> gameObject);
	void DeleteHierarchy(engine::Component& transform);
};

