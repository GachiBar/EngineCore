#include "Application.h"

#include <iostream>

#include "Delegates.h"
#include "Layer.h"
#include "InputSystem/InputManager.h"

Application::Application():m_LayerStack(this),engine_(new engine::Engine()), exit_code_(0)
{
}

void Application::PushLayer(Layer* layer)
{
	m_LayerStack.PushLayer(layer);
	layer->OnAttach();
}

void Application::PushOverlay(Layer* layer)
{
	m_LayerStack.PushOverlay(layer);
	layer->OnAttach();
}

int Application::Run()
{
	Setup();
	if (exit_code_)
		return exit_code_;

	engine_->Initialize();

	Start();
	if (exit_code_)
		return exit_code_;

	while (!engine_->IsExiting())
	{
		ApplyInput();
		engine_->RunFrame();
	}

	Stop();

	engine_->Terminate();

	return exit_code_;
}

std::shared_ptr<engine::Engine> Application::GetEngine()
{
	return engine_;
}

void Application::ApplyInput()
{
	std::shared_ptr<InputEvent> CurrentEvent= InputManager::getInstance().GetMouseDevice().Read();
	while (CurrentEvent)
	{
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (CurrentEvent->Handled)
				break;
			(*it)->OnInputEvent(CurrentEvent.get());
		}
		CurrentEvent = CurrentEvent = InputManager::getInstance().GetMouseDevice().Read();
	}

	CurrentEvent = InputManager::getInstance().GetKeyboardDevice().Read();
	while (CurrentEvent)
	{
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (CurrentEvent->Handled)
				break;
			(*it)->OnInputEvent(CurrentEvent.get());
		}
		CurrentEvent = CurrentEvent = InputManager::getInstance().GetKeyboardDevice().Read();
	}
}
