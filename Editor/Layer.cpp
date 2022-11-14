#include "Layer.h"

Layer::Layer(LayerStack* owner, std::string name)
	: CurrentInputMode(EEditorInputMode::Type::EditorAndGameMode), owner_layer_stack(owner), m_DebugName(std::move(name))
{
}

Application* Layer::GetApp() const
{
	//TODO add log and asserts
	return owner_layer_stack->GetOwner();
}

EEditorInputMode::Type Layer::GetCurrentInputMode() const
{
	return CurrentInputMode;
}
