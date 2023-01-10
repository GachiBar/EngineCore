#pragma once
#include <memory>

#include "EditorCamera.h"
#include "GameViewWindow.h"
#include "Layer.h"
#include "LogWindow.h"
#include "MenuBar.h"
#include "GameObjectInspectorWindow.h"
#include "SceneHierarchyWindow.h"
#include "ProjectSettingsWindow.h"
#include "Resources/MaterialsEditor.h"

class PropertyWindow;
class ExplorerWindow;
class AIEditorWindow;

class EditorLayer final : public Layer
{
public:
	EditorLayer(LayerStack* owner, EditorCamera& editor—amera);
	~EditorLayer() override = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate(float const dt) override;
	virtual void OnGuiRender() override;
	virtual void OnPostRender() override;

	void SetBlockEvents(bool block) { m_BlockEvents = block; }

	std::shared_ptr<GameViewWindow> gvm;
	std::shared_ptr<SceneHierarchyWindow> hierarchy;
	std::shared_ptr<PropertyWindow> property_window;
	std::shared_ptr<GameObjectInspectorWindow> game_object_inspector;
	std::shared_ptr<MenuBar> menu_bar;
	std::shared_ptr<ProjectSettingsWindow> SettingsWindow;
	std::shared_ptr<ExplorerWindow> explorer;
	std::shared_ptr<LogWindow> log;
	std::shared_ptr<MaterialsEditor> resourceDrawer;
	std::shared_ptr<AIEditorWindow> ai_editor;

	engine::GameObject* GetSelectedGo();

private:

	EditorCamera& editor_camera;
	std::shared_ptr<engine::GameObject> selected_go;

	bool m_BlockEvents = true;
	bool m_ShowPhysicsColliders = false;
	bool m_ViewportFocused = false, m_ViewportHovered = false;
	DirectX::SimpleMath::Vector2 m_ViewportSize = { 0.0f, 0.0f };
	std::pair<DirectX::SimpleMath::Vector2, DirectX::SimpleMath::Vector2> m_ViewportBounds;

	void OpenScene();
	void SaveScene();
};

