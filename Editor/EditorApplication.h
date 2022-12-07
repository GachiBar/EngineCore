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

	std::shared_ptr<EditorCamera> Camera;
	
	void OnEnterGameMode();
	void OnExitGameMode();
	
private:
	EEditorInputMode::Type editor_input_mode;

	std::shared_ptr<GameLayer> game_layer;
	std::shared_ptr<EditorLayer> editor_layer;
};
