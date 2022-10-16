#pragma once
#include "Layer.h"


class EditorLayer final : public Layer
{
public:
	EditorLayer(LayerStack* owner);
	~EditorLayer() override = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	//virtual void OnInputEvent(InputEvent* e) override;
	virtual void OnGuiRender() override;


	void SetBlockEvents(bool block) { m_BlockEvents = block; }
private:
	bool m_BlockEvents = true;
};

