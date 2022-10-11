#pragma once

#include <string>

#include "InputSystem/InputEvent.h"


class Layer
{
public:
	Layer(std::string name = "Layer");
	virtual ~Layer() = default;

	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnUpdate(float const dt) {}
	virtual void OnGuiRender() {}
	virtual void OnInputEvent(InputEvent& event) {}

	const std::string& GetName() const { return m_DebugName; }
protected:
	std::string m_DebugName;
};
