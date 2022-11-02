#include "ProjectSettingsWindow.h"

#include <string>

#include "imgui/imgui.h"
#include "InputSystem/InputManager.h"
#include "InputSystem/UnrealCoreSystem/InputSettings.h"
#include "libs/imgui_sugar.hpp"

void ProjectSettingsWindow::draw_imgui()
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
								ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!\n\n");
								ImGui::Separator();

								char buff[256];
								strcpy_s(buff, ActionMapping.first.c_str());
								static std::string result;
								ImGui::InputText("RenameValue", buff, 256,ImGuiInputTextFlags_CallbackEdit,
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
									EKeys::GetAllKeys(Keys);
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
								with_ID(("clear_button_"+Map.ToString()).c_str())
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
					if(SelectedKey!=EKeys::Invalid)
					{
						InputManager::getInstance().input_settings->AddActionMapping(ActionOrAxisName, SelectedKey);
						InputManager::getInstance().input_settings->RemoveActionMapping(ActionOrAxisName, UnSelectedKey);
					}
					if(bWasRemoveActionMapping)
					{
						InputManager::getInstance().input_settings->RemoveActionMapping(ActionOrAxisName, SelectedKey);
					}
					if(bWasRemoveAction)
					{
						InputManager::getInstance().input_settings->RemoveAction(ActionOrAxisName);
					}
					if(bWasAddActionMapping)
					{
						InputManager::getInstance().input_settings->AddActionMapping(ActionOrAxisName, EKeys::Invalid);
					}
					if(bWasRenaming)
						InputManager::getInstance().input_settings->RenameActionGroup(ActionOrAxisName, ActionOrAxisNameNew);
				}

				with_TreeNode("Axis Mappings")
				{
					if (ImGui::Button(plus.c_str()))
					{

						std::cout << "Hello";
					}

					ImGui::SameLine();
					if (ImGui::Button(clear.c_str()))
					{

					}
				}

				ImGui::Text("IsItemHovered: %d", ImGui::IsItemHovered());
				for (int i = 0; i < 5; i++)
					ImGui::Text("Some content %d", i);
			}
		}
	}
}
