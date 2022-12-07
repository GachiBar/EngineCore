#pragma once

#include <string>

#include "Application.h"
#include "../Core/Interfaces/ILayer.h"

struct FInputEvent;
class LayerStack;

class Layer : public ILayer 
{
public:
	Layer(LayerStack* owner, std::string name = "Layer");
	virtual ~Layer() = default;

	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnUpdate(float const dt) {}
	virtual void OnGuiRender() {}
	virtual void OnPostRender() {}
	EEditorInputMode::Type GetCurrentInputMode() const;

	//virtual void OnInputEvent(FInputEvent* event) {}

	const std::string& GetName() const { return m_DebugName; }

	Application* GetApp() const;

	
protected:
	EEditorInputMode::Type CurrentInputMode;

	LayerStack* owner_layer_stack;
	std::string m_DebugName;
};
