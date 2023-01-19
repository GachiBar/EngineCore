#include "GameLayer.h"

#include <iostream>

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
	, is_playing(false)
{
	CurrentInputMode = EEditorInputMode::Type::GameOnlyMode;
}

void GameLayer::OnAttach()
{
	Layer::OnAttach();
}

void GameLayer::OnUpdate(float const dt)
{
	Layer::OnUpdate(dt);

	if (GetIsPlaying()) 
	{
		GetApp()->GetEngine()->RunFrame();
	}	
}
