#include <iostream>

#include "../GameplaySystem/Engine.h"


//TODO Move application header to another project
#include "../Editor/Application.h"
#include "../Editor/EditorApplication.h"
#include "../monowrapper/monopp/mono_jit.cpp"

class StandaloneGameTestApplication final : public Application {
public:
	StandaloneGameTestApplication() 
		: Application()
		, scene(nullptr)
	{}

	~StandaloneGameTestApplication() {
		if (scene != nullptr) {
			delete scene;
		}	
	}

	Scene* scene = nullptr;
	std::shared_ptr<GameObject> game_object_1;
	std::shared_ptr<GameObject>  game_object_2;

	void OnSetup() override;
};


void StandaloneGameTestApplication::OnSetup() {
	scene = new Scene(m_Assembly);
	game_object_1 = scene->CreateGameObject();
	game_object_2 = scene->CreateGameObject();

	game_object_1->AddComponent("GameplayCore.Components", "MeshRenderComponent");
	game_object_1->AddComponent("GameplayCore.Components", "TransformComponent");

	engine_->SetScene(scene);
}

int main() {
	//StandaloneGameTestApplication app(kDllPath);
	EditorApplication app;
	return app.Run();
}
