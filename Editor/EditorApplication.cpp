#include "EditorApplication.h"
#include "EditorLayer.h"
#include "../GameplaySystem/Component.h"

#include <iostream>

#include "GameLayer.h"
#include "InputManager.h"
#include "InputCoreSystem/InputSettings.h"
#include "Windows/GenericWindow.h"

EditorApplication::EditorApplication()
	: Application()
	  , Camera(new EditorCamera()), editor_input_mode(EEditorInputMode::Type::EditorOnlyMode)
{
}

void EditorApplication::OnSetup()
{
	Application::OnSetup();

	InputManager::getInstance().input_settings->AddDefaultEditorExclusiveKeys();
}

void EditorApplication::OnStart()
{
	Application::OnStart();
	const auto game_layer = new GameLayer(&m_LayerStack);
	PushLayer(game_layer);

	const auto editor_layer = new EditorLayer(&m_LayerStack);
	Camera->owner_layer = editor_layer;
	PushLayer(editor_layer);
	editor_layer->gvm->EnteringGameMode.AddRaw(this, &EditorApplication::SetGameOnlyOnlyInputMode);
	editor_layer->gvm->ExitGameMode.AddRaw(this, &EditorApplication::SetEditorOnlyInputMode);

	GetMainWindow()->WindowSizeChangedEvent.AddRaw(editor_layer->gvm.get(), &GameViewWindow::on_resize_viewport);
}

EEditorInputMode::Type EditorApplication::GetCurrentInputMode() const
{
	return editor_input_mode;
}

engine::GameObject* EditorApplication::GetCamera() const
{
	return camera_go.get();
}

void EditorApplication::SetEditorOnlyInputMode()
{
	editor_input_mode = EEditorInputMode::Type::EditorOnlyMode;
}

void EditorApplication::SetGameOnlyOnlyInputMode()
{
	editor_input_mode = EEditorInputMode::Type::GameOnlyMode;
}
