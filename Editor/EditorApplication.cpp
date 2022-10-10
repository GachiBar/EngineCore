#include "EditorApplication.h"

EditorApplication::EditorApplication():Application(),mw(new MainWindow)
{
}

void EditorApplication::Setup()
{
	AddComponent(go1, "GameplayCore", "TestUpdateComponent");
	AddComponent(go2, "GameplayCore", "TestFixedUpdateComponent");

	engine_->SetScene(&scene);
}
