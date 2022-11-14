#pragma once

#include "../GameplaySystem/Scene.h"
#include "../GameplaySystem/GameObject.h"
#include "libs/Delegates.h"

#include <vector>

#include "Interfaces/IApplication.h"

class SceneHierarchyWindow
{
public:
	SceneHierarchyWindow(const mono::mono_assembly& assembly);

	void draw_imgui();

	DECLARE_EVENT(GameObjectSelectedEvent, SceneHierarchyWindow, std::shared_ptr<engine::GameObject>)
	GameObjectSelectedEvent GameObjectSelected;

	engine::Scene* GetScene() const;

	IApplication* app;
private:

	std::shared_ptr<engine::GameObject> selected;
	const mono::mono_assembly& assembly;

	void BuildTree(engine::Component& transform);

	bool IsSelected(std::shared_ptr<engine::GameObject> gameObject);
};

