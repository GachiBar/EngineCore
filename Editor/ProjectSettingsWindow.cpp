#include "ProjectSettingsWindow.h"

#include <string>

#include "imgui/imgui.h"
#include "InputManager.h"
#include "InputCoreSystem/InputSettings.h"
#include "libs/imgui_sugar.hpp"
#include "winApiFileLoad.h"
#include "FilePathsHelper.h"
#include "ConfigReaderWriterFactory.h"
#include <format>
#include <filesystem>

ProjectSettingsWindow::ProjectSettingsWindow()
{
	ConfigReaderWriterFactory factory;
	config_reader_writer = factory.Create();
}

ProjectSettingsWindow::~ProjectSettingsWindow()
{
	delete config_reader_writer;
}

void ProjectSettingsWindow::Draw()
{
	static std::string selected = "";

	with_Window("Project settings", nullptr, ImGuiWindowFlags_MenuBar)
	{

		with_Child("##SettingsHierarchy", ImVec2(ImGui::GetContentRegionAvail().x * 0.2f, ImGui::GetContentRegionAvail().y), false, ImGuiWindowFlags_HorizontalScrollbar)
		{
			ImGui::SetNextItemOpen(true);
			with_TreeNodeEx("Engine", ImGuiWindowFlags_NoMouseInputs)
			{
				ImGui::SetNextItemOpen(true);

				ImGui::TreeNodeEx("Input", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen);

				if(ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
					int a = 42;
			}
		}
		ImGui::SameLine();

		with_Child("##SettingsContent")
		{
			with_CollapsingHeader("Input", ImGuiTreeNodeFlags_None)
			{
				bool bWasChanges = draw_action_mappings_menu();
				bWasChanges|=draw_axis_mappings_menu();
				if(bWasChanges)
				{
					InputManager::getInstance().input_settings->SaveKeyMappingsToFile();
				}				
			}

			with_CollapsingHeader("Game settings", ImGuiTreeNodeFlags_None)
			{			
				const size_t nameSize = 256;
				char name[nameSize] = {};

				auto fromConfigPath = config_reader_writer->GetValue<std::string>("Game settings", "Default name");
				auto configName = fromConfigPath.value_or("");

				strncpy_s(name, configName.data(), nameSize);

				if (ImGui::InputText("##Default name", name, nameSize, ImGuiInputTextFlags_ReadOnly))
				{
					config_reader_writer->SetValue("Game settings", "Default name", name);
				}

				ImGui::SameLine();

				if (ImGui::Button("..."))
				{
					auto inConfigPaths = LoadFileFromExplorer(L"Scenes", L"Scenes|*.dat");

					if (inConfigPaths.empty()) 
					{
						return;
					}
			
					auto inConfigPath = std::filesystem::relative(inConfigPaths[0], std::filesystem::current_path());
					config_reader_writer->SetValue("Game settings", "Default name", inConfigPath.generic_string());
				}

				ImGui::SameLine();
				ImGui::LabelText("Default name", "Default name");
			}
		}
	}
}

bool ProjectSettingsWindow::draw_action_mappings_menu()
{
	std::string plus = "+";
	std::string clear = "-";
	std::string rename = "r";

	with_TreeNode("Action Mappings")
	{
		if (ImGui::Button(plus.c_str()))
		{
			InputManager::getInstance().input_settings->AddActionGroup(InputManager::getInstance().input_settings->GetUniqueActionName("NewActionMapping_"));
		}

		ImGui::SameLine();
		//clear all actions
		if (ImGui::Button(clear.c_str()))
		{
			InputManager::getInstance().input_settings->RemoveAllActions();
		}

		FKey SelectedKey = EKeys::Invalid;
		FKey UnSelectedKey = EKeys::Invalid;
		static std::string ActionOrAxisName;
		std::string ActionOrAxisNameNew;

		bool bWasAddActionMapping = false;
		bool bWasRemoveAction = false;
		bool bWasRemoveActionMapping = false;
		bool bWasRenaming = false;
		for (auto& ActionMapping : InputManager::getInstance().input_settings->GetActionMappings())
		{
			with_TreeNode(ActionMapping.first.c_str())
			{
				//Add action mappings to group
				if (ImGui::Button(plus.c_str()))
				{
					bWasAddActionMapping = true;
					ActionOrAxisName = ActionMapping.first;
				}

				ImGui::SameLine();
				//clear action group
				if (ImGui::Button(clear.c_str()))
				{
					bWasRemoveAction = true;
					ActionOrAxisName = ActionMapping.first;
				}

				ImGui::SameLine();
				//rename action group
				if (ImGui::Button(rename.c_str()))
				{
					// Always center this window when appearing

					ActionOrAxisName = ActionMapping.first;
					ImGui::OpenPopup("rename?");
				}

				ImVec2 center = ImGui::GetMainViewport()->GetCenter();
				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
				if (ImGui::BeginPopupModal("rename?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					char buff[256];
					strcpy_s(buff, ActionMapping.first.c_str());
					static std::string result;
					ImGui::InputText("RenameValue", buff, 256, ImGuiInputTextFlags_CallbackEdit,
						[](ImGuiInputTextCallbackData* data)
						{
							result = std::string(data->Buf);
							return 0;
						});

					if (ImGui::Button("OK", ImVec2(120, 0)))
					{
						ActionOrAxisNameNew = result;
						bWasRenaming = true;
						//InputManager::getInstance().input_settings->RenameActionGroup(ActionMapping.first, result);
						ImGui::CloseCurrentPopup();
					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}

				for (auto& Map : ActionMapping.second)
				{
					with_Combo(("###ActionMapping_" + Map.ToString()).c_str(), Map.ToString().c_str(), ImGuiComboFlags_NoArrowButton)
					{
						std::vector<FKey> Keys;
						EKeys::GetAllKeys(Keys,FKeyDetails::EKeyFlags::MouseButton | FKeyDetails::EKeyFlags::KeyboardKey);
						for (auto& Key : Keys)
						{
							if (ImGui::Selectable(Key.ToString().c_str(), Key == Map))
							{
								ActionOrAxisName = ActionMapping.first;
								SelectedKey = Key;
								UnSelectedKey = Map;
							}

							// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
							if (Key == Map)
								ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::SameLine();
					//clear action mapping
					with_ID(("clear_button_" + Map.ToString()).c_str())
					{
						if (ImGui::Button(clear.c_str()))
						{
							bWasRemoveActionMapping = true;
							ActionOrAxisName = ActionMapping.first;
							SelectedKey = Map;
						}
					}
				}

			}
		}
		if (SelectedKey != EKeys::Invalid)
		{
			InputManager::getInstance().input_settings->AddActionMapping(ActionOrAxisName, SelectedKey);
			InputManager::getInstance().input_settings->RemoveActionMapping(ActionOrAxisName, UnSelectedKey);
		}
		if (bWasRemoveActionMapping)
		{
			InputManager::getInstance().input_settings->RemoveActionMapping(ActionOrAxisName, SelectedKey);
		}
		if (bWasRemoveAction)
		{
			InputManager::getInstance().input_settings->RemoveAction(ActionOrAxisName);
		}
		if (bWasAddActionMapping)
		{
			InputManager::getInstance().input_settings->AddActionMapping(ActionOrAxisName, EKeys::Invalid);
		}
		if (bWasRenaming)
			InputManager::getInstance().input_settings->RenameActionGroup(ActionOrAxisName, ActionOrAxisNameNew);

		return bWasRemoveActionMapping || bWasRenaming || bWasAddActionMapping || bWasRemoveAction || SelectedKey != EKeys::Invalid;
	}

	return false;
}

bool ProjectSettingsWindow::draw_axis_mappings_menu()
{
	std::string plus = "+";
	std::string clear = "-";
	std::string rename = "r";

	with_TreeNode("Axis Mappings")
	{
		if (ImGui::Button(plus.c_str()))
		{
			InputManager::getInstance().input_settings->AddAxisGroup(InputManager::getInstance().input_settings->GetUniqueAxisName("NewAxisMapping_"));
		}

		ImGui::SameLine();
		//clear all actions
		if (ImGui::Button(clear.c_str()))
		{
			InputManager::getInstance().input_settings->RemoveAllAxis();
		}

		FKey SelectedKey = EKeys::Invalid;
		FKey UnSelectedKey = EKeys::Invalid;
		static std::string ActionOrAxisName;
		std::string ActionOrAxisNameNew;

		bool bWasAddActionMapping = false;
		bool bWasRemoveAction = false;
		bool bWasRemoveActionMapping = false;
		bool bWasRenaming = false;
		for (auto& AxisMapping : InputManager::getInstance().input_settings->GetAxisMappings())
		{
			with_TreeNode(AxisMapping.first.c_str())
			{
				//Add action mappings to group
				if (ImGui::Button(plus.c_str()))
				{
					bWasAddActionMapping = true;
					ActionOrAxisName = AxisMapping.first;
				}

				ImGui::SameLine();
				//clear action group
				if (ImGui::Button(clear.c_str()))
				{
					bWasRemoveAction = true;
					ActionOrAxisName = AxisMapping.first;
				}

				ImGui::SameLine();
				//rename action group
				if (ImGui::Button(rename.c_str()))
				{
					// Always center this window when appearing

					ActionOrAxisName = AxisMapping.first;
					ImGui::OpenPopup("rename?");
				}

				ImVec2 center = ImGui::GetMainViewport()->GetCenter();
				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
				if (ImGui::BeginPopupModal("rename?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					char buff[256];
					strcpy_s(buff, AxisMapping.first.c_str());
					static std::string result;
					ImGui::InputText("###RenameValue", buff, 256, ImGuiInputTextFlags_CallbackEdit,
						[](ImGuiInputTextCallbackData* data)
						{
							result = std::string(data->Buf);
							return 0;
						});

					if (ImGui::Button("OK", ImVec2(120, 0)))
					{
						ActionOrAxisNameNew = result;
						bWasRenaming = true;
						//InputManager::getInstance().input_settings->RenameActionGroup(ActionMapping.first, result);
						ImGui::CloseCurrentPopup();
					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}

				for (auto& Map : AxisMapping.second)
				{
					with_Combo(("###AxisMapping_" + Map.Key.ToString()).c_str(), Map.Key.ToString().c_str(), ImGuiComboFlags_NoArrowButton)
					{
						std::vector<FKey> Keys;
						EKeys::GetAllKeys(Keys);
						for (auto& Key : Keys)
						{
							if (ImGui::Selectable(Key.ToString().c_str(), Key == Map.Key))
							{
								ActionOrAxisName = AxisMapping.first;
								SelectedKey = Key;
								UnSelectedKey = Map.Key;
							}

							// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
							if (Key == Map.Key)
								ImGui::SetItemDefaultFocus();
						}
					}
					

					ImGui::SameLine();
					//clear action mapping
					with_ID(("clear_button_" + Map.Key.ToString()).c_str())
					{
						if (ImGui::Button(clear.c_str()))
						{
							bWasRemoveActionMapping = true;
							ActionOrAxisName = AxisMapping.first;
							SelectedKey = Map.Key;
						}
					}
					ImGui::SameLine();
					ImGui::InputFloat(("###AxisMappingScale_" + Map.Key.ToString()).c_str(), const_cast<float*>(&Map.Scale));
					
				}

			}
		}
		if (SelectedKey != EKeys::Invalid)
		{
			InputManager::getInstance().input_settings->AddAxisMapping(ActionOrAxisName, SelectedKey);
			InputManager::getInstance().input_settings->RemoveAxisMapping(ActionOrAxisName, UnSelectedKey);
		}
		if (bWasRemoveActionMapping)
		{
			InputManager::getInstance().input_settings->RemoveAxisMapping(ActionOrAxisName, SelectedKey);
		}
		if (bWasRemoveAction)
		{
			InputManager::getInstance().input_settings->RemoveAxis(ActionOrAxisName);
		}
		if (bWasAddActionMapping)
		{
			InputManager::getInstance().input_settings->AddAxisMapping(ActionOrAxisName, EKeys::Invalid);
		}
		if (bWasRenaming)
			InputManager::getInstance().input_settings->RenameAxisGroup(ActionOrAxisName, ActionOrAxisNameNew);
		return bWasRemoveActionMapping || bWasRenaming || bWasAddActionMapping || bWasRemoveAction || SelectedKey != EKeys::Invalid;
	}
	return false;
}
