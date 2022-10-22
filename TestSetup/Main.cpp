#include <iostream>

#include "../GameplaySystem/Engine.h"


//TODO Move application header to another project
#include "../Editor/Application.h"
#include "../Editor/EditorApplication.h"

const char kMonoLibPath[] = "..\\vendor\\mono\\lib\\4.5";
const char kDllPath[] = "GameplayCore.dll";

class StandaloneGameTestApplication final : public Application
{
public:
	StandaloneGameTestApplication(const char* dll_path) 
		: Application(dll_path)
	{}

	Scene scene;
	GameObject* game_object_1 = nullptr;
	GameObject* game_object_2 = nullptr;

	void OnSetup() override;
};


void StandaloneGameTestApplication::OnSetup()
{
	scene = Scene(m_Assembly);
	game_object_1 = scene.CreateGameObject();
	game_object_2 = scene.CreateGameObject();

	game_object_1->AddComponent(m_Assembly, "GameplayCore.Components", "TestUpdateComponent");
	game_object_1->AddComponent(m_Assembly, "GameplayCore.Components", "TransformComponent");
	game_object_1->AddComponent(m_Assembly, "GameplayCore.Components", "MeshRenderComponent");

	game_object_2->AddComponent(m_Assembly, "GameplayCore.Components", "TestFixedUpdateComponent");

	engine_->SetScene(&scene);
}
#include "../Editor/Delegates.h"

int main() {
	StandaloneGameTestApplication app(kDllPath);
	//EditorApplication app(kDllPath);
	return app.Run();
}
