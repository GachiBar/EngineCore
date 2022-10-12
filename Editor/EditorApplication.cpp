#include "EditorApplication.h"
#include "EditorLayer.h"

EditorApplication::EditorApplication():Application() //,mw(new MainWindow)
{
}

void EditorApplication::Setup()
{
	AddComponent(go1, "GameplayCore", "TestUpdateComponent");
	AddComponent(go2, "GameplayCore", "TestFixedUpdateComponent");

	engine_->SetScene(&scene);

	const auto editor_layer = new EditorLayer(&m_LayerStack);
	PushLayer(editor_layer);
}
