#pragma once
#include "GameViewWindow.h"
#include "Layer.h"
#include "LogWindow.h"
#include "MenuBar.h"
#include "GameObjectInspectorWindow.h"
#include "SceneHierarchyWindow.h"
#include "ProjectSettingsWindow.h"

class ExplorerWindow;

class EditorLayer final : public Layer
{
public:
	EditorLayer(LayerStack* owner);
	~EditorLayer() override = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate(float const dt) override;
	//virtual void OnInputEvent(InputEvent* e) override;
	virtual void OnGuiRender() override;
	virtual void OnPostRender() override;

	void SetBlockEvents(bool block) { m_BlockEvents = block; }

	std::shared_ptr<GameViewWindow> gvm;
	std::shared_ptr<SceneHierarchyWindow> hierarchy;
	std::shared_ptr<GameObjectInspectorWindow> properties;
	std::shared_ptr<MenuBar> menu_bar;
	std::shared_ptr<ProjectSettingsWindow> SettingsWindow;
	std::shared_ptr<ExplorerWindow> explorer;
	std::shared_ptr<LogWindow> log;

	engine::GameObject* GetSelectedGo();

private:

	std::shared_ptr<engine::GameObject> editor_camera;
	std::shared_ptr<engine::GameObject> selected_go;

	bool m_BlockEvents = true;
	bool m_ShowPhysicsColliders = false;
	bool m_ViewportFocused = false, m_ViewportHovered = false;
	DirectX::SimpleMath::Vector2 m_ViewportSize = { 0.0f, 0.0f };
	std::pair<DirectX::SimpleMath::Vector2, DirectX::SimpleMath::Vector2> m_ViewportBounds;
};

