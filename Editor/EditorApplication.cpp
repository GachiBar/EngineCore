#include "EditorApplication.h"
#include "EditorLayer.h"
#include "../GameplaySystem/Component.h"

#include <iostream>

EditorApplication::EditorApplication()
	: Application() //,mw(new MainWindow)
	, scene(nullptr)
{
}

void EditorApplication::OnSetup()
{
	Application::OnSetup();

	scene = std::make_shared<engine::Scene>(m_Assembly);

	camera_go = scene->CreateGameObject();
	test_go = scene->CreateGameObject();

	auto game_object_1 = scene->CreateGameObject();
	auto game_object_2 = scene->CreateGameObject();


	game_object_1->AddComponent("GameplayCore.Components", "MeshRenderComponent");
	game_object_1->AddComponent("GameplayCore.Components", "TransformComponent");	
	game_object_1->AddComponent("GameplayCore.Components", "TestEditorFieldsComponent");

	game_object_2->AddComponent("GameplayCore.Components", "CameraComponent");
	game_object_2->AddComponent("GameplayCore.Components", "TransformComponent");


	camera_go->AddComponent("GameplayCore.Components", "CameraComponent");
	camera_go->AddComponent("GameplayCore.Components", "TransformComponent");


	test_go->AddComponent("GameplayCore.Components", "MeshRenderComponent");
	test_go->AddComponent("GameplayCore.Components", "TransformComponent");

	//game_object_3->AddComponent("GameplayCore.Components", "MeshRenderComponent");
	//auto transform = game_object_3->AddComponent("GameplayCore.Components", "TransformComponent");
	//auto property = transform->GetProperty("LocalPosition");
	//DirectX::SimpleMath::Vector3 position(0, 0, 1);
	//property.SetValue(&position);

	for (size_t i = 0; i < scene->Count(); ++i) {
		std::cout << "go:" << i << "\n";
		auto go = (*scene)[i];

		for (size_t j = 0; j < go->Count(); ++j) {
			std::cout << "\t" << "component" << j << "\n";
			auto component = (*go)[j];
		}
	}

	engine_->SetScene(scene);

}

void EditorApplication::OnStart()
{
	Application::OnStart();
	const auto editor_layer = new EditorLayer(&m_LayerStack);
	PushLayer(editor_layer);
}

engine::GameObject* EditorApplication::GetCamera() const
{
	return camera_go.get();
}
