#pragma once

#include "Application.h"
#include "EditorCamera.h"
#include "EditorLayer.h"
#include "GameLayer.h"

class EditorApplication final : public Application
{
public:
	EditorApplication();

	void OnSetup() override;

	void OnStart() override;

	EEditorInputMode::Type GetCurrentInputMode() const override;
	
private:
	EEditorInputMode::Type editor_input_mode;
	GameLayer* game_layer;
	EditorLayer* editor_layer;
	std::shared_ptr<engine::Scene> scene;
	std::shared_ptr<engine::Scene> scene_copy;

	void SetupGameImGui();
	void OnEnterGameMode();
	void OnExitGameMode();
	void OnViewportPresented();
};
