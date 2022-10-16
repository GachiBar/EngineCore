#include "EditorApplication.h"
#include "EditorLayer.h"

EditorApplication::EditorApplication():Application() //,mw(new MainWindow)
{
}

void EditorApplication::OnSetup()
{
	Application::OnSetup();

	AddComponent(go1, "GameplayCore.Components", "TestUpdateComponent");
	AddComponent(go2, "GameplayCore.Components", "TestFixedUpdateComponent");
	AddComponent(go1, "GameplayCore.Components", "RenderComponent");
	AddComponent(go1, "GameplayCore.Components", "TransformComponent");

	engine_->SetScene(&scene);

}

void EditorApplication::OnStart()
{
	Application::OnStart();
	const auto editor_layer = new EditorLayer(&m_LayerStack);
	PushLayer(editor_layer);
}
