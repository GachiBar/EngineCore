#include "GameLayer.h"
#include "../GameplaySystem/GameObject.h"

#include <iostream>

#include "imgui/imgui_internal.h"

bool GameLayer::GetIsPlaying() const {
	return is_playing;
}

void GameLayer::SetIsPlaying(bool value) {
	is_playing = value;
	
	if (is_playing) 
	{
		GetApp()->GetEngine()->Start();
	}
	else
	{
		GetApp()->GetEngine()->Stop();
	}
}

GameLayer::GameLayer(LayerStack* owner)
	: Layer(owner, "GameLayer")
	, scene(new engine::Scene())
	, is_playing(false)
{
	CurrentInputMode = EEditorInputMode::Type::GameOnlyMode;
}

void GameLayer::OnAttach()
{
	Layer::OnAttach();

	auto method = scene->GetType().GetMethod("SetupImGui", 3);

	ImGuiMemAllocFunc memAllocFunc[1];
	ImGuiMemFreeFunc memFreeFunc[1];

	void* userData;
	ImGui::GetAllocatorFunctions(memAllocFunc, memFreeFunc, &userData);

	void* params[3];
	params[0] = &GImGui;
	params[1] = &memAllocFunc;
	params[2] = &memFreeFunc;

	method.Invoke(*scene, params);

	auto game_object_1 = scene->CreateGameObject();
	auto game_object_2 = scene->CreateGameObject();
	auto game_object_3 = scene->CreateGameObject();
	scene->Invalidate();

	game_object_1->AddComponent("GameplayCore.Components", "MeshRenderComponent");
	auto transform = game_object_1->AddComponent("GameplayCore.Components", "TransformComponent");
	game_object_1->AddComponent("GameplayCore.Components", "TestEditorFieldsComponent");
	game_object_1->Invalidate();

	game_object_2->AddComponent("GameplayCore.Components", "CameraComponent");
	game_object_2->AddComponent("GameplayCore.Components", "TransformComponent");
	game_object_2->AddComponent("GameplayCore.Components", "SpectatorComponent");
	game_object_2->Invalidate();

	for (size_t i = 0; i < scene->Count(); ++i) {
		std::cout << "go:" << i << "\n";
		auto go = (*scene)[i];

		for (size_t j = 0; j < go->Count(); ++j) {
			std::cout << "\t" << "component" << j << "\n";
			auto component = (*go)[j];
		}
	}

	GetApp()->GetEngine()->SetScene(scene);
	scene->Initialize();
}

void GameLayer::OnUpdate(float const dt)
{
	Layer::OnUpdate(dt);

	if (GetIsPlaying()) 
	{
		GetApp()->GetEngine()->RunFrame();
	}	
}

void GameLayer::OnGuiRender()
{
	GetApp()->DrawGameUI();
}

void GameLayer::OnPostRender()
{
	Layer::OnPostRender();
}
