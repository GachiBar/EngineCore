#pragma once
#include "Layer.h"

class GameLayer final : public Layer
{
public:
	GameLayer(LayerStack* owner);
	~GameLayer() override = default;

	virtual void OnAttach() override;
	//virtual void OnDetach() override;
	void OnUpdate(float const dt) override;
	//virtual void OnInputEvent(InputEvent* e) override;
	virtual void OnGuiRender() override;
	virtual void OnPostRender() override;
protected:
	std::shared_ptr<engine::Scene> scene;
};
