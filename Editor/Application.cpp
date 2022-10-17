#include "Application.h"

#include <iostream>

#include "Delegates.h"
#include "Layer.h"
#include "InputSystem/InputManager.h"

Application::Application(const char* dll_path)
	: m_LayerStack(this)
	, m_Domain{ "KtripDomain" }
	, m_Assembly{ m_Domain, dll_path }
	, engine_(new engine::Engine(m_Domain, m_Assembly))
	, exit_code_(0)
{
	mono::mono_domain::set_current_domain(m_Domain);
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

mono::mono_object Application::CreateGameObject(const mono::mono_object& scene) {
	mono::mono_method create_go_method(scene.get_type(), "CreateGameObject", 0);
	mono::mono_method_invoker create_go_method_invoker(create_go_method);
	mono::mono_object go(create_go_method_invoker.invoke(scene));

	mono::mono_method invalidate_scene_method(scene.get_type(), "Invalidate", 0);
	mono::mono_method_invoker invalidate_scene_method_invoker(invalidate_scene_method);
	invalidate_scene_method_invoker.invoke(scene);

	return go;
}

mono::mono_object Application::AddComponent(
	const mono::mono_assembly& assembly, 
	const mono::mono_object& go, 
	const std::string& name_space, 
	const std::string& name)
{
	mono::mono_method add_component_method(go.get_type(), "AddComponent", 1);
	mono::mono_method_invoker add_component_method_invoker(add_component_method);

	MonoDomain* domain = mono::mono_domain::get_current_domain().get_internal_ptr();

	mono::mono_type component_type = assembly.get_type(name_space, name);
	MonoType* type = mono_class_get_type(component_type.get_internal_ptr());
	MonoReflectionType* reflection_type = mono_type_get_object(domain, type);

	void* params[1];
	params[0] = reflection_type;

	mono::mono_object component(add_component_method_invoker.invoke(go, params));

	mono::mono_method invalidate_go_method(go.get_type(), "Invalidate", 0);
	mono::mono_method_invoker invalidate_go_method_invoker(invalidate_go_method);
	invalidate_go_method_invoker.invoke(go);

	return component;
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
