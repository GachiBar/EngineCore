#pragma once

#include "IEditorWindow.h"
#include "imgui/imgui.h"
#include "../Core/libs/Delegates.h"
#include "../GameplaySystem/Runtime.h"
#include "../GameplaySystem/Scene.h"
#include "../GameplaySystem/GameObject.h"

#include <vector>
#include <functional>

class SceneHierarchyWindow : public IEditorWindow
{
public:
	std::shared_ptr<engine::Scene> GetScene();
	void SetScene(std::shared_ptr<engine::Scene> scene);
	
	std::string GetSceneName();
	void SetSceneName(std::string scene_name);

	SceneHierarchyWindow();

	void Draw() override;

	DECLARE_EVENT(GameObjectSelectedEvent, SceneHierarchyWindow, std::shared_ptr<engine::GameObject>)
	GameObjectSelectedEvent GameObjectSelected;

private:

#pragma region drag&drop handlers

	class DragAndDropHandler
	{
	public:
		virtual void Handle(std::shared_ptr<engine::GameObject> source) = 0;
	};

	class WithTransformDragAndDropHandler : public DragAndDropHandler 
	{
	public:
		virtual void Handle(std::shared_ptr<engine::GameObject> source) override;
	};

	class WithoutTransformDragAndDropHandler : public DragAndDropHandler
	{
	public:
		virtual void Handle(std::shared_ptr<engine::GameObject> source) override;
	};

	class HierarchyDragAndDropHandler : public DragAndDropHandler
	{
	public:
		HierarchyDragAndDropHandler(std::shared_ptr<engine::GameObject> target);
		virtual void Handle(std::shared_ptr<engine::GameObject> source) override;
	private:
		std::shared_ptr<engine::GameObject> target;
	};

#pragma endregion drag&drop handlers

	class DragAndDropRequest
	{
	public:
		std::shared_ptr<engine::GameObject> Source = nullptr;
		std::shared_ptr<DragAndDropHandler> Handler;

		bool IsRequested();
		void Reset();
		void Handle();
	};	

	int tree_level_id;
	DragAndDropRequest drag_and_drop_request;

	std::string scene_name;
	std::shared_ptr<engine::Scene> scene;
	std::shared_ptr<engine::GameObject> selected;

	void DrawWithTransformHeaderPopup();
	void DrawWithoutTransformHeaderPopup();
	void DrawWithTransformPopup(std::shared_ptr<engine::GameObject> gameObject);
	void DrawWithoutTransformPopup(std::shared_ptr<engine::GameObject> gameObject);

	void DrawWithTransform(const std::vector<std::shared_ptr<engine::GameObject>>& roots);
	void DrawHierarchy(engine::Component& transform);
	void DrawSelected(std::shared_ptr<engine::GameObject> gameObject);	

	void DrawWithoutTransform(const std::vector<std::shared_ptr<engine::GameObject>>& transformless);

	bool IsSelected(std::shared_ptr<engine::GameObject> gameObject);
	std::shared_ptr<engine::GameObject> CopyHierarchy(engine::GameObject& gameObject);
	void DeleteHierarchy(engine::Component& transform);

	void SetupDragSource(std::shared_ptr<engine::GameObject> gameObject);

	template<typename THandler, typename ...Args>
	void SetupDropTarget(Args&&... args)
	{
		if (ImGui::BeginDragDropTarget())
		{
			if (ImGui::AcceptDragDropPayload("_TREENODE"))
			{
				drag_and_drop_request.Handler = std::make_shared<THandler>(args...);
			}

			ImGui::EndDragDropTarget();
		}
	}

	void ProcessDragAndDrop();

	void SortGameObjects(
		std::vector<std::shared_ptr<engine::GameObject>>& roots_out,
		std::vector<std::shared_ptr<engine::GameObject>>& transformless_out);
};

