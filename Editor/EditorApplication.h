#pragma once

#include "Application.h"
#include "EditorCamera.h"
#include "EditorLayer.h"
#include "GameLayer.h"

class ConfigReaderWriter;

class EditorApplication final : public Application
{
public:
	EditorApplication();
	~EditorApplication();

	void OnSetup() override;

	void OnStart() override;

	EEditorInputMode::Type GetCurrentInputMode() const override;
	
private:
	ConfigReaderWriter* config_reader_writer;
	EEditorInputMode::Type editor_input_mode;
	GameLayer* game_layer;
	EditorLayer* editor_layer;
	std::shared_ptr<engine::Scene> scene;
	std::string serialized_scene;

	bool TryLoadDefaultScene();
	void SetupGameImGui();
	void OnEnterGameMode();
	void OnExitGameMode();
	void OnViewportPresented();
};
