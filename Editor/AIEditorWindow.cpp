#include "AIEditorWindow.h"
#include "Resources/MetadataReader.h"
#include "../GameplaySystem/Object.h"
#include "../GameplaySystem/Runtime.h"

#include <unordered_set>

AIEditorWindow::AIEditorWindow() 
{
	CacheActionsData();
	available_actions_items = new const char* [actions_names.size()];
}

AIEditorWindow::~AIEditorWindow() 
{
	delete[] available_actions_items;
}

void AIEditorWindow::Draw()
{
	if (asset == nullptr || actions_list == nullptr) 
	{
		ImGui::Text("Nothing selected");
	}

	DrawExistingActions();
	DrawAddActionPanel();
}

bool AIEditorWindow::TrySelect(const std::filesystem::path& path)
{
	auto assetOpt = MetadataReader::read_internal(path);

	if (!assetOpt.has_value())
	{
		asset = nullptr;
		return false;
	}

	asset = std::make_shared<engine::Object>(std::move(assetOpt.value()));
	asset->GetType().GetMethod("Load").Invoke(*asset);
	auto actionsListOpt = asset->GetType().GetField("_actions").GetValue(*asset);

	if (!actionsListOpt.has_value()) 
	{
		actions_list = nullptr;
		return false;
	}

	actions_list = std::make_shared<engine::Object>(std::move(actionsListOpt.value()));
	FindAvaliableActions();
	return true;
}

void AIEditorWindow::SaveAsset()
{
	auto saveMethod = asset->GetType().GetMethod("Save");
	saveMethod.Invoke(*asset);
}

size_t AIEditorWindow::GetCount()
{
	auto countProperty = actions_list->GetType().GetProperty("Count");
	auto countOpt = countProperty.GetValue(*actions_list);
	auto count = countOpt.value();
	return count.Unbox<size_t>();
}

engine::Object AIEditorWindow::GetAction(size_t index)
{
	auto getMethod = actions_list->GetType().GetMethod("get_Item", 1);

	void* params[1];
	params[0] = &index;

	auto actionOpt = getMethod.Invoke(*actions_list, params);
	return actionOpt.value();
}

void AIEditorWindow::AddAction(engine::Object action)
{
	auto addMethod = actions_list->GetType().GetMethod("Add", 1);

	void* params[1];
	params[0] = action.GetInternal().get_internal_ptr();

	addMethod.Invoke(*actions_list, params);
}

void AIEditorWindow::RemoveAction(engine::Object action)
{
	auto removeMethod = actions_list->GetType().GetMethod("Remove", 1);

	void* params[1];
	params[0] = action.GetInternal().get_internal_ptr();

	removeMethod.Invoke(*actions_list, params);
}

void AIEditorWindow::DrawExistingActions()
{
	for (size_t i = 0; i < GetCount(); ++i)
	{
		auto action = GetAction(i);
		auto actionType = action.GetType();
		auto actionName = actionType.GetFullName();

		bool visible = true;
		ImGui::CollapsingHeader(actionName.c_str(), &visible, ImGuiTreeNodeFlags_Bullet);

		if (!visible)
		{
			i -= 1;
			RemoveAction(action);
			SaveAsset();
			FindAvaliableActions();
			auto temp = 0;
		}
	}
}

void AIEditorWindow::DrawAddActionPanel()
{
	int selected = 0;

	if (ImGui::Combo("Add", &selected, available_actions_items, avaliable_actions_count))
	{
		std::string fullName(available_actions_items[selected]);
		auto type = engine::Runtime::GetCurrentRuntime().GetType(fullName);
		auto action = type.Instantiate();
		AddAction(action);
		SaveAsset();
		FindAvaliableActions();
	}
}

void AIEditorWindow::CacheActionsData() 
{
	auto& runtime = engine::Runtime::GetCurrentRuntime();
	auto baseComponentType = runtime.GetType(engine::Types::kAIAction);
	auto typeNames = runtime.DumpTypeNames();

	for (auto typeName : typeNames)
	{
		try
		{
			auto typeDeclarartion = engine::Types::ParseFullName(typeName);
			auto type = engine::Runtime::GetCurrentRuntime().GetType(typeDeclarartion);

			while (type.HasBaseType() && !type.IsAbstract())
			{
				if (type.IsDerivedFrom(baseComponentType))
				{
					actions_names.push_back(typeName);
					break;
				}

				type = type.GetBaseType();
			}
		}
		catch (mono::mono_exception& ex)
		{
			continue;
		}
	}
}

void AIEditorWindow::FindAvaliableActions()
{
	std::unordered_set<std::string> addedComponents;

	for (size_t i = 0; i < GetCount(); ++i)
	{
		auto action = GetAction(i);
		addedComponents.insert(action.GetType().GetFullName());
	}

	avaliable_actions_count = actions_names.size() - addedComponents.size();
	auto temp = actions_names.begin();

	for (size_t i = 0; i < avaliable_actions_count; ++i)
	{
		while (addedComponents.contains(*temp))
		{
			std::advance(temp, 1);
		}

		available_actions_items[i] = temp->data();
		std::advance(temp, 1);
	}
}
