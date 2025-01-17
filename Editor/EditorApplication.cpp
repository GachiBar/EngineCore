#include "EditorApplication.h"
#include "InputManager.h"
#include "InputCoreSystem/InputSettings.h"
#include "Windows/GenericWindow.h"
#include "ConfigReaderWriterFactory.h"
#include "FilePathsHelper.h"

#include <imgui/imgui_internal.h>

EditorApplication::EditorApplication()
	: Application()
	, editor_input_mode(EEditorInputMode::Type::EditorOnlyMode)
	, game_layer(nullptr)
	, editor_layer(nullptr)
	, scene(new engine::Scene())
{
	ConfigReaderWriterFactory factory;
	config_reader_writer = factory.Create();
}

EditorApplication::~EditorApplication()
{
	delete config_reader_writer;
}

void EditorApplication::OnSetup()
{
	Application::OnSetup();

	InputManager::getInstance().input_settings->AddDefaultEditorExclusiveKeys();
}

void EditorApplication::OnStart()
{
	Application::OnStart();
	SetupGameImGui();
	engine_->SetScene(scene);

	game_layer = new GameLayer(&m_LayerStack);
	PushLayer(game_layer);

	editor_layer =new EditorLayer(&m_LayerStack);
	PushLayer(editor_layer);

	editor_layer->gvm->EnterGameMode.AddRaw(this, &EditorApplication::OnEnterGameMode);
	editor_layer->gvm->ExitGameMode.AddRaw(this, &EditorApplication::OnExitGameMode);
	editor_layer->gvm->ViewportPresented.AddRaw(this, &EditorApplication::OnViewportPresented);

	GetMainWindow()->WindowSizeChangedEvent.AddRaw(editor_layer->gvm.get(), &GameViewWindow::OnResizeViewport);
	TryLoadDefaultScene();
}

EEditorInputMode::Type EditorApplication::GetCurrentInputMode() const
{
	return editor_input_mode;
}

bool EditorApplication::TryLoadDefaultScene()
{
	auto pathOpt = config_reader_writer->GetValue<std::string>("Game settings", "Default name");

	if (pathOpt.has_value())
	{
		auto path = pathOpt.value();
		std::fstream file(path, std::fstream::in);

		if (!file.fail())
		{
			std::string json;
			getline(file, json, '\0');
			scene->Deserialize(json);
			auto default_scene_name = FilePahtsHelper::RemoveExtension(FilePahtsHelper::BaseName(path));
			editor_layer->SetScenePath(FilePahtsHelper::s2ws(path));
			return true;
		}
	}

	return false;
}

void EditorApplication::SetupGameImGui()
{
	auto& runtime = engine::Runtime::GetCurrentRuntime();
	auto type = runtime.GetType(engine::Types::kImGuiApi);
	auto method = type.GetMethod("SetupImGui", 3);

	ImGuiMemAllocFunc memAllocFunc[1];
	ImGuiMemFreeFunc memFreeFunc[1];

	void* userData;
	ImGui::GetAllocatorFunctions(memAllocFunc, memFreeFunc, &userData);

	void* params[3];
	params[0] = &GImGui;
	params[1] = &memAllocFunc;
	params[2] = &memFreeFunc;

	method.Invoke(params);
}

void EditorApplication::OnEnterGameMode()
{
	editor_input_mode = EEditorInputMode::Type::GameOnlyMode;
	serialized_scene = scene->Serialize();
	engine_->GetScene()->Initialize();
	game_layer->SetIsPlaying(true);
}

void EditorApplication::OnExitGameMode()
{
	editor_input_mode = EEditorInputMode::Type::EditorOnlyMode;
	game_layer->SetIsPlaying(false);
	engine_->GetScene()->Terminate();
	scene->Deserialize(serialized_scene);
	editor_layer->game_object_inspector->SetGameObject(nullptr);
}

void EditorApplication::OnViewportPresented()
{
	if (editor_layer->gvm->IsPlaying()) 
	{
		DrawGameUI();
	}	
}