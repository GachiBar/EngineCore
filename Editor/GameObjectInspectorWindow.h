#pragma once

#include "IEditorWindow.h"
#include "ObjectDrawer.h"
#include "../GameplaySystem/Runtime.h"
#include "../GameplaySystem/Scene.h"
#include "../GameplaySystem/GameObject.h"
#include "../GameplaySystem/Field.h"

#include <memory>
#include <unordered_set>

class GameObjectInspectorWindow : public IEditorWindow
{
public:
	std::shared_ptr<engine::Scene> GetScene() const;
	void SetScene(std::shared_ptr<engine::Scene> scene);

	std::shared_ptr<engine::GameObject> GetGameObject();
	void SetGameObject(std::shared_ptr<engine::GameObject> gameObject);

	GameObjectInspectorWindow();

	~GameObjectInspectorWindow() override;

	void Draw() override;

private:
	ObjectDrawer object_drawer;

	std::shared_ptr<engine::GameObject> game_object;

	const char** available_components_items;
	size_t avaliable_components_count;

	std::vector<std::string> components_names;

	void DrawGameObjectFields();
	void DrawComponentFields(std::shared_ptr<engine::Component> component);
	void DrawAddComponentPanel();

	void FindAvaliableComponents();
};
