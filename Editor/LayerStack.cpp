#include "LayerStack.h"

#include "Layer.h"

LayerStack::LayerStack(Application* _owner):owner(_owner)
{
}

LayerStack::~LayerStack()
{
	for (Layer* layer : layers_)
	{
		layer->OnDetach();
		delete layer;
	}
}

void LayerStack::PushLayer(Layer* layer)
{
	layers_.emplace(layers_.begin() + m_LayerInsertIndex, layer);
	m_LayerInsertIndex++;
}

void LayerStack::PushOverlay(Layer* overlay)
{
	layers_.emplace_back(overlay);
}

void LayerStack::PopLayer(Layer* layer)
{
	const auto it = std::find(layers_.begin(), layers_.begin() + m_LayerInsertIndex, layer);
	if (it != layers_.begin() + m_LayerInsertIndex)
	{
		layer->OnDetach();
		layers_.erase(it);
		m_LayerInsertIndex--;
	}
}

void LayerStack::PopOverlay(Layer* overlay)
{
	const auto it = std::find(layers_.begin() + m_LayerInsertIndex, layers_.end(), overlay);
	if (it != layers_.end())
	{
		overlay->OnDetach();
		layers_.erase(it);
	}
}

Application* LayerStack::GetOwner() const
{
	return owner;
}
