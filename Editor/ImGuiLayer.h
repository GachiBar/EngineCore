#pragma once
#include "Layer.h"

class ImGuiLayer final : public Layer
{
public:
	ImGuiLayer(LayerStack* owner);
	~ImGuiLayer() override = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnInputEvent(FInputEvent* e) override;

	void Begin();
	void End();

	void SetBlockEvents(bool block) { m_BlockEvents = block; }
private:
	bool m_BlockEvents = true;
};
