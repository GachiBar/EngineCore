#pragma once
#include "Layer.h"

class GameLayer final : public Layer
{
public:
	bool GetIsPlaying() const;
	void SetIsPlaying(bool value);

	GameLayer(LayerStack* owner);
	~GameLayer() override = default;

	virtual void OnAttach() override;
	void OnUpdate(float const dt) override;

protected:	
	bool is_playing;
};
