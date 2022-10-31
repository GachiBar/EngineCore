#pragma once
#include "GameViewWindow.h"
#include "Layer.h"
#include "MenuBar.h"
#include "PropertyWindow.h"
#include "SceneHierarchyWindow.h"


class EditorLayer final : public Layer
{
public:
	EditorLayer(LayerStack* owner);
	~EditorLayer() override = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	//virtual void OnInputEvent(InputEvent* e) override;
	virtual void OnGuiRender() override;


	void SetBlockEvents(bool block) { m_BlockEvents = block; }

	std::shared_ptr<GameViewWindow> gvm;
	std::shared_ptr<SceneHierarchyWindow> hierarchy;
	std::shared_ptr<PropertyWindow> properties;
	std::shared_ptr<MenuBar> menu_bar;
	
	
private:
	std::shared_ptr<engine::GameObject> selected_go;

	bool m_BlockEvents = true;
	bool m_ShowPhysicsColliders = false;
	bool m_ViewportFocused = false, m_ViewportHovered = false;
	DirectX::SimpleMath::Vector2 m_ViewportSize = { 0.0f, 0.0f };
	std::pair<DirectX::SimpleMath::Vector2, DirectX::SimpleMath::Vector2> m_ViewportBounds;
};

