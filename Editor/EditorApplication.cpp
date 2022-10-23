#include "EditorApplication.h"
#include "EditorLayer.h"
#include "../monowrapper/monopp/mono_property.h"
#include "../monowrapper/monopp/mono_property_invoker.h"

EditorApplication::EditorApplication():Application() //,mw(new MainWindow)
{
}

void EditorApplication::OnSetup()
{
	Application::OnSetup();

	scene = new Scene(m_Assembly);
	game_object_1 = scene->CreateGameObject();
	game_object_2 = scene->CreateGameObject();
	
	game_object_1->AddComponent("GameplayCore.Components", "TestUpdateComponent");
	game_object_1->AddComponent("GameplayCore.Components", "TransformComponent");
	game_object_1->AddComponent("GameplayCore.Components", "MeshRenderComponent");

	game_object_2->AddComponent("GameplayCore.Components", "TestFixedUpdateComponent");

	engine_->SetScene(scene);

}

void EditorApplication::OnStart()
{
	Application::OnStart();
	const auto editor_layer = new EditorLayer(&m_LayerStack);
	PushLayer(editor_layer);
}

void EditorApplication::OnStop()
{
	// TODO: we should clean up scene
	//delete scene;
}
