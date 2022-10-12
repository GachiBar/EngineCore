#pragma once
#include <vector>

class Application;
class Layer;

class LayerStack
{
public:
	LayerStack(Application* _owner);
	virtual ~LayerStack();

	void PushLayer(Layer* layer);
	void PushOverlay(Layer* overlay);
	void PopLayer(Layer* layer);
	void PopOverlay(Layer* overlay);

	std::vector<Layer*>::iterator begin() { return layers_.begin(); }
	std::vector<Layer*>::iterator end() { return layers_.end(); }
	std::vector<Layer*>::reverse_iterator rbegin() { return layers_.rbegin(); }
	std::vector<Layer*>::reverse_iterator rend() { return layers_.rend(); }

	std::vector<Layer*>::const_iterator begin() const { return layers_.begin(); }
	std::vector<Layer*>::const_iterator end()	const { return layers_.end(); }
	std::vector<Layer*>::const_reverse_iterator rbegin() const { return layers_.rbegin(); }
	std::vector<Layer*>::const_reverse_iterator rend() const { return layers_.rend(); }

	Application* GetOwner() const;
protected:
	Application* owner;
	std::vector<Layer*> layers_;
	unsigned int m_LayerInsertIndex = 0;
};
