#include "Layer.h"

Layer::Layer(LayerStack* owner, std::string name)
	: owner_layer_stack(owner),m_DebugName(std::move(name))
{
}
