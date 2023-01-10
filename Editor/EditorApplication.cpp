#include "EditorApplication.h"
#include "../GameplaySystem/Component.h"
#include "InputManager.h"
#include "InputCoreSystem/InputSettings.h"
#include "Windows/GenericWindow.h"

EditorApplication::EditorApplication()
	: Application()
	, Camera(new EditorCamera())
	, editor_input_mode(EEditorInputMode::Type::EditorOnlyMode)
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
	game_layer = new GameLayer(&m_LayerStack);
	PushLayer(game_layer);

	editor_layer =new EditorLayer (&m_LayerStack, *Camera);
	Camera->owner_layer = editor_layer;
	PushLayer(editor_layer);
	editor_layer->gvm->EnterGameMode.AddRaw(this, &EditorApplication::OnEnterGameMode);
	editor_layer->gvm->ExitGameMode.AddRaw(this, &EditorApplication::OnExitGameMode);

	GetMainWindow()->WindowSizeChangedEvent.AddRaw(editor_layer->gvm.get(), &GameViewWindow::on_resize_viewport);
}

EEditorInputMode::Type EditorApplication::GetCurrentInputMode() const
{
	return editor_input_mode;
}

void EditorApplication::OnEnterGameMode()
{
	editor_input_mode = EEditorInputMode::Type::GameOnlyMode;
	game_layer->SetIsPlaying(true);
}

void EditorApplication::OnExitGameMode()
{
	editor_input_mode = EEditorInputMode::Type::EditorOnlyMode;
	game_layer->SetIsPlaying(false);	
}
