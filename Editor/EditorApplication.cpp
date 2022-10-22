#include "EditorApplication.h"
#include "EditorLayer.h"
#include "../monowrapper/monopp/mono_property.h"
#include "../monowrapper/monopp/mono_property_invoker.h"

EditorApplication::EditorApplication(const char* dll_path):Application(dll_path) //,mw(new MainWindow)
{
}

void EditorApplication::OnSetup()
{
	Application::OnSetup();

	scene = Scene(m_Assembly);
	game_object_1 = scene.CreateGameObject();
	game_object_2 = scene.CreateGameObject();
	
	game_object_1->AddComponent(m_Assembly, "GameplayCore.Components", "TestUpdateComponent");
	game_object_1->AddComponent(m_Assembly, "GameplayCore.Components", "TransformComponent");
	game_object_1->AddComponent(m_Assembly, "GameplayCore.Components", "MeshRenderComponent");

	game_object_2->AddComponent(m_Assembly, "GameplayCore.Components", "TestFixedUpdateComponent");

	engine_->SetScene(&scene);

}

void EditorApplication::OnStart()
{
	Application::OnStart();
	const auto editor_layer = new EditorLayer(&m_LayerStack);
	PushLayer(editor_layer);
}
