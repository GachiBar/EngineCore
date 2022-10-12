#pragma once

#include <string>

#include "InputSystem/InputEvent.h"


class LayerStack;

class Layer
{
public:
	Layer(LayerStack* owner, std::string name = "Layer");
	virtual ~Layer() = default;

	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnUpdate(float const dt) {}
	virtual void OnGuiRender() {}
	virtual void OnInputEvent(InputEvent* event) {}

	const std::string& GetName() const { return m_DebugName; }
protected:
	LayerStack* owner_layer_stack;
	std::string m_DebugName;
};