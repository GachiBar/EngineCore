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

GameObjectInspectorWindow::GameObjectInspectorWindow(const mono::mono_assembly& assembly)
	: assembly(assembly)
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
	auto baseComponentType = assembly.get_type("GameplayCore.Components", "Component");
	auto typeNames = assembly.dump_type_names();

	for (auto typeName : typeNames)
	{
		mono::mono_type type;

		try
		{
			std::string nameSpace;
			std::string name;
			ParseFullName(typeName, nameSpace, name);
			type = assembly.get_type(nameSpace, name);
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
		object_drawer.DrawObject(*component);
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
		std::string nameSpace;
		std::string name;
		ParseFullName(fullName, nameSpace, name);

		game_object->AddComponent(nameSpace, name);	
		game_object->Invalidate();
		FindAvaliableComponents();
	}
}

void GameObjectInspectorWindow::ParseFullName(
	const std::string& fullName, 
	std::string& namespace_out, 
	std::string& name_out)
{
	size_t lastDotPosition = fullName.find_last_of(".");
	namespace_out = fullName.substr(0, lastDotPosition);
	name_out = fullName.substr(lastDotPosition + 1, fullName.size() - lastDotPosition - 1);
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
