#include "Layer.h"

Layer::Layer(LayerStack* owner, std::string name)
	: owner_layer_stack(owner),m_DebugName(std::move(name))
{
}

Application* Layer::GetApp() const
{
	//TODO add log and asserts
	return owner_layer_stack->GetOwner();
}
