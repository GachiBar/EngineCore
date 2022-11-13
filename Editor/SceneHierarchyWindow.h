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
	struct DragAndDropRequest
	{
		std::shared_ptr<engine::GameObject> source = nullptr;
		std::shared_ptr<engine::GameObject> target = nullptr;

		bool IsRequested();
		void Reset();
	};
		
	int tree_level_id;
	DragAndDropRequest drag_and_drop_request;

	std::shared_ptr<engine::Scene> scene;
	std::shared_ptr<engine::GameObject> selected;
	const mono::mono_assembly& assembly;

	void DrawWithTransformHeaderPopup();

	void DrawHierarchy(engine::Component& transform);
	void DrawSelected(std::shared_ptr<engine::GameObject> gameObject);
	void SetupDragSource(std::shared_ptr<engine::GameObject> gameObject);
	void SetupDropSource(std::shared_ptr<engine::GameObject> gameObject);
	void DrawPopup(std::shared_ptr<engine::GameObject> gameObject);

	bool IsSelected(std::shared_ptr<engine::GameObject> gameObject);
	void DeleteHierarchy(engine::Component& transform);

	void ProcessWithTransformHeaderDragAndDrop();
	void ProcessHierarchyDragAndDrop();
};

