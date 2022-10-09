#pragma once

#include <string>

class Layer
{
public:
	Layer(std::string name = "Layer");
	virtual ~Layer() = default;

	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnUpdate(float const dt) {}
	virtual void OnGuiRender() {}
	//virtual void OnEvent(Event& event) {}

	const std::string& GetName() const { return m_DebugName; }
protected:
	std::string m_DebugName;
};
