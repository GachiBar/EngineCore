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

GameObjectInspectorWindow::GameObjectInspectorWindow()
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
	auto& runtime = engine::Runtime::GetCurrentRuntime();
	auto baseComponentType = runtime.GetType(engine::Types::kComponent);
	auto typeNames = runtime.DumpTypeNames();

	for (auto typeName : typeNames)
	{
		try
		{
			auto typeDeclarartion = engine::Types::ParseFullName(typeName);
			auto type = engine::Runtime::GetCurrentRuntime().GetType(typeDeclarartion);
			bool isComponent = false;

			while (type.HasBaseType() && !type.IsAbstract())
			{
				if (type.IsDerivedFrom(baseComponentType))
				{
					isComponent = true;
					break;
				}

				type = type.GetBaseType();
			}

			if (isComponent)
			{
				components_names.push_back(typeName);
			}
		}
		catch (mono::mono_exception& ex)
		{
			continue;
		}
	}
}

void GameObjectInspectorWindow::DrawGameObjectFields()
{
	if (ImGui::CollapsingHeader("GameObject", ImGuiTreeNodeFlags_DefaultOpen))
	{
		std::vector<std::string> modifiedFields;
		object_drawer.DrawObject(*game_object, modifiedFields);
		ImGui::NewLine();
	}	
}

void GameObjectInspectorWindow::DrawComponentFields(std::shared_ptr<engine::Component> component)
{	
	bool visible = true;

	if (ImGui::CollapsingHeader(component->Name().c_str(), &visible, ImGuiTreeNodeFlags_DefaultOpen))
	{
		std::vector<std::string> modifiedFields;

		if (object_drawer.DrawObject(*component, modifiedFields)) 
		{
			if (component->GetType().HasMethod("Invalidate", 1)) 
			{
				auto invalidate = component->GetType().GetMethod("Invalidate", 1);

				for (auto field : modifiedFields)
				{
					auto& domain = mono::mono_domain::get_current_domain();
					mono::mono_string fieldName(domain, field);

					void* params[1];
					params[0] = fieldName.get_internal_ptr();

					invalidate.Invoke(*component, params);
				}				
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
		game_object->AddComponent(fullName);
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
