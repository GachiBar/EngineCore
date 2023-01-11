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
	GetApp()->GetEngine()->SetScene(scene);
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
	//if (GetIsPlaying()) 
	//{
	//	GetApp()->DrawGameUI();
	//}	
}

void GameLayer::OnPostRender()
{
	Layer::OnPostRender();
}
