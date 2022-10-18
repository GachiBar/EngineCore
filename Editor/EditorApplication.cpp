#include "EditorApplication.h"
#include "EditorLayer.h"

EditorApplication::EditorApplication(const char* dll_path):Application(dll_path) //,mw(new MainWindow)
{
}

void EditorApplication::OnSetup()
{
	Application::OnSetup();
	AddComponent(m_Assembly, go1, "GameplayCore.Components", "TestUpdateComponent");
	AddComponent(m_Assembly, go2, "GameplayCore.Components", "TestFixedUpdateComponent");
	AddComponent(m_Assembly, go1, "GameplayCore.Components", "RenderComponent");
	AddComponent(m_Assembly, go1, "GameplayCore.Components", "TransformComponent");

	engine_->SetScene(&scene);

}

void EditorApplication::OnStart()
{
	Application::OnStart();
	const auto editor_layer = new EditorLayer(&m_LayerStack);
	PushLayer(editor_layer);
}
