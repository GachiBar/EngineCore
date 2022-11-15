#pragma once

#include "Application.h"
#include "EditorCamera.h"

class EditorApplication final : public Application
{
public:
	EditorApplication();

	void OnSetup() override;

	void OnStart() override;

	EEditorInputMode::Type GetCurrentInputMode() const override;

	engine::GameObject* GetCamera() const;

	std::shared_ptr < EditorCamera> Camera;
	
	void SetEditorOnlyInputMode();

	void SetGameOnlyOnlyInputMode();
	
protected:
	EEditorInputMode::Type editor_input_mode;

	std::shared_ptr<engine::GameObject> camera_go;
};
