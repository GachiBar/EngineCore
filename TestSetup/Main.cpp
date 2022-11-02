#include <iostream>
#include <DirectXTex.h>

#include "../GameplaySystem/Engine.h"

//TODO Move application header to another project
#include "../Editor/Application.h"
#include "../Editor/EditorApplication.h"
#include "../GameplaySystem/TextureLoader.h"

class StandaloneGameTestApplication final : public Application {
public:
	StandaloneGameTestApplication() 
		: Application()
		, scene(nullptr)
	{}

	std::shared_ptr<engine::Scene> scene = nullptr;

	void OnSetup() override;
};


void StandaloneGameTestApplication::OnSetup() {
	scene = std::make_shared<engine::Scene>(m_Assembly);
	auto game_object_1 = scene->CreateGameObject();
	auto game_object_2 = scene->CreateGameObject();

	game_object_1->AddComponent("GameplayCore.Components", "MeshRenderComponent");
	game_object_1->AddComponent("GameplayCore.Components", "TransformComponent");

	engine_->SetScene(scene);
}

int main() {
	if (HRESULT result = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED); FAILED(result)) {
		return result;
	}

	//StandaloneGameTestApplication app(kDllPath);
	EditorApplication app;
	return app.Run();
}
