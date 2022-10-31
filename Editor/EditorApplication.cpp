#include "EditorApplication.h"
#include "EditorLayer.h"
#include "../monowrapper/monopp/mono_property.h"
#include "../monowrapper/monopp/mono_property_invoker.h"
#include <iostream>

EditorApplication::EditorApplication()
	: Application() //,mw(new MainWindow)
	, scene(nullptr)
{
}

EditorApplication::~EditorApplication()
{
	if (scene != nullptr) 
	{
		delete scene;
	}
}

void EditorApplication::OnSetup()
{
	Application::OnSetup();

	scene = new Scene(m_Assembly);
	game_object_1 = scene->CreateGameObject();
	game_object_2 = scene->CreateGameObject();

	game_object_1->AddComponent("GameplayCore.Components", "MeshRenderComponent");
	game_object_1->AddComponent("GameplayCore.Components", "TransformComponent");	

	game_object_2->AddComponent("GameplayCore.Components", "CameraComponent");
	game_object_2->AddComponent("GameplayCore.Components", "TransformComponent");

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
