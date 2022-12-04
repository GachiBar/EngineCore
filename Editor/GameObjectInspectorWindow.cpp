#include "GameObjectInspectorWindow.h"
#include "../GameplaySystem/Component.h"
#include "../GameplaySystem/Engine.h"
#include "../monowrapper/monopp/mono_domain.h"
#include "../monowrapper/monopp/mono_method_invoker.h"
#include "../monowrapper/monopp/mono_string.h"

#include <mono/metadata/reflection.h>
#include <imgui/imgui.h>
#include <SimpleMath.h>
#include <format>

std::shared_ptr<engine::Scene> GameObjectInspectorWindow::GetScene() const
{
	return object_drawer.GetScene();
}

void GameObjectInspectorWindow::SetScene(std::shared_ptr<engine::Scene> scene)
{
	object_drawer.SetScene(scene);
}

std::shared_ptr<engine::GameObject> GameObjectInspectorWindow::GetGameObject()
{
	return game_object;
}

void GameObjectInspectorWindow::SetGameObject(std::shared_ptr<engine::GameObject> gameObject)
{
	this->game_object = gameObject;

	if (game_object != nullptr) 
	{
		FindAvaliableComponents();
	}	
}

GameObjectInspectorWindow::GameObjectInspectorWindow(const engine::Runtime& runtime)
	: runtime(runtime)
{
	CacheComponentsData();
	available_components_items = new const char* [components_names.size()];
}

GameObjectInspectorWindow::~GameObjectInspectorWindow()
{
	delete[] available_components_items;
}

void GameObjectInspectorWindow::Draw()
{
	ImGui::Begin("Property Window");
	if (!game_object.get())
	{
		ImGui::End();
		return;
	}

	DrawGameObjectFields();

	for (size_t i = 0; i < game_object->Count(); ++i)
	{
		auto component = (*game_object)[i];
		DrawComponentFields(component);
	}

	ImGui::Separator();
	DrawAddComponentPanel();
	ImGui::End();	
}

void GameObjectInspectorWindow::CacheComponentsData()
{
	auto baseComponentType = runtime.GetAssembly().get_type("GameplayCore.Components", "Component");
	auto typeNames = runtime.GetAssembly().dump_type_names();

	for (auto typeName : typeNames)
	{
		mono::mono_type type;

		try
		{
			auto typeDeclaration = engine::Types::ParseFullName(typeName);
			type = runtime.GetAssembly().get_type(typeDeclaration.name_space, typeDeclaration.name);
		}
		catch (mono::mono_exception& ex)
		{
			continue;
		}

		bool isComponent = false;

		while (type.has_base_type() && !type.is_abstract())
		{
			if (type.is_derived_from(baseComponentType))
			{
				isComponent = true;
				break;
			}
			
			type = type.get_base_type();
		}

		if (isComponent)
		{
			components_names.push_back(type.get_fullname());
		}
	}
}

void GameObjectInspectorWindow::DrawGameObjectFields()
{
	if (ImGui::CollapsingHeader("GameObject", ImGuiTreeNodeFlags_DefaultOpen))
	{
		object_drawer.DrawObject(*game_object);
		ImGui::NewLine();
	}	
}

void GameObjectInspectorWindow::DrawComponentFields(std::shared_ptr<engine::Component> component)
{	
	bool visible = true;

	if (ImGui::CollapsingHeader(component->Name().c_str(), &visible, ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (object_drawer.DrawObject(*component)) 
		{
			//if (isFieldChanged && component->HasMethod("Invalidate", 1))
			//{
			//	auto& domain = mono::mono_domain::get_current_domain();
			//	mono::mono_string fieldName(domain, field.GetName());

			//	void* params[1];
			//	params[0] = fieldName.get_internal_ptr();

			//	component->GetMethod("Invalidate", 1).Invoke(params);
			//}

			if (component->GetType().HasMethod("Invalidate")) 
			{
				component->GetType().GetMethod("Invalidate").Invoke();
			}
		}

		ImGui::NewLine();
	}
	
	if (!visible)
	{
		game_object->RemoveComponent(component);
		game_object->Invalidate();
		FindAvaliableComponents();
	}
}

void GameObjectInspectorWindow::DrawAddComponentPanel()
{
	int selected = 0;

	if (ImGui::Combo("Add", &selected, available_components_items, avaliable_components_count))
	{		
		std::string fullName(available_components_items[selected]);
		auto typeDeclaration = engine::Types::ParseFullName(fullName);
		game_object->AddComponent(typeDeclaration.name_space, typeDeclaration.name);
		game_object->Invalidate();
		FindAvaliableComponents();
	}
}

void GameObjectInspectorWindow::FindAvaliableComponents()
{
	std::unordered_set<std::string> addedComponents;

	for (size_t i = 0; i < game_object->Count(); ++i)
	{
		auto component = (*game_object)[i];
		auto componentType = component->GetInternal().get_type();
		addedComponents.insert(componentType.get_fullname());
	}

	avaliable_components_count = components_names.size() - addedComponents.size();
	auto temp = components_names.begin();

	for (size_t i = 0; i < avaliable_components_count; ++i)
	{
		while (addedComponents.contains(*temp))
		{
			std::advance(temp, 1);
		}

		available_components_items[i] = temp->data();
		std::advance(temp, 1);
	}
}
