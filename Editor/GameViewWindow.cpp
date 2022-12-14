#include "GameViewWindow.h"

#include "EditorApplication.h"
#include "imgui/imgui.h"
#include "libs/MathUtility.h"
#include "EditorLayer.h"
#include "InputManager.h"
#include "ImGuizmo/ImGuizmo.h"
#include "libs/imgui_sugar.hpp"
#include "../GameplaySystem/Component.h"
#include "libs/magic_enum.hpp"
#include <ranges>
#include <iostream>

#include "imgui/imgui_internal.h"

GameViewWindow::GameViewWindow(void* InTexture, EditorLayer* InEditorLayer): Texture(InTexture),
                                                                             editor_layer(InEditorLayer)
{
	SelectedRenderTarget = *editor_layer->GetApp()->GetEngine()->GetRenderer().GetRenderTargetsList().rbegin();

	LogManager::getInstance().ViewportMessageAdded.AddRaw(this, &GameViewWindow::OnLogMessageAdded);
	LogManager::getInstance().ViewportMessageRemoved.AddRaw(this, &GameViewWindow::OnLogMessageRemoved);
}

void GameViewWindow::update()
{
	if(bInFocus && !IsInCameraEditorInputMode())
	{
		const auto input = InputManager::getInstance().player_input;
		if (input->WasJustPressed(EKeys::W))
			CurrentGizmoOperation = ImGuizmo::TRANSLATE;
		if (input->WasJustPressed(EKeys::E))
			CurrentGizmoOperation = ImGuizmo::ROTATE;
		if (input->WasJustPressed(EKeys::R))
			CurrentGizmoOperation = ImGuizmo::SCALE;
		if (input->WasJustPressed(EKeys::C))
			SwitchOperationMode();
	}
}


static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs)            { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }

void GameViewWindow::Draw()
{
	with_Window("Game Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse
		| ImGuiWindowFlags_MenuBar)
	{

		//ZDES VSEO UI

		if (ImGui::Button("TeastButton", {200,100}))
		{
			std::cout<<"PressedTest"<<std::endl<<std::flush;
		};
		
		with_MenuBar
		{
			if (ImGui::MenuItem("Play", "", bIsPlaying, !bIsPlaying))
			{
				StartPlay();
			}
			if (ImGui::MenuItem("Stop", "", !bIsPlaying, bIsPlaying))
			{
				StopPlay();
			}
		}

		with_Child("GameRender", {}, false,  ImGuiWindowFlags_NoBackground)
		{
			{
				ImGui::GetWindowDrawList()->AddImage(Texture,ImGui::GetWindowPos(), ImGui::GetWindowPos()+ImGui::GetWindowSize());
			
				editor_layer->GetApp()->DrawGameUI();
			}
			
			ImVec2 p0 = ImGui::GetItemRectMin();
			const ImVec2 p1 = ImGui::GetItemRectMax();

			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			ImGui::PushClipRect(p0, p1, true);

			for (int i = 0; i < guid_verbosity_messages.size(); ++i)
			{
				auto Color = LogManager::getInstance().GetLevelLogColor(std::get<1>(guid_verbosity_messages[i]));
				draw_list->AddText(p0, IM_COL32(Color.x*255, Color.y * 255, Color.z * 255, Color.w * 255), std::get<2>(guid_verbosity_messages[i]).c_str());
				p0.y += draw_list->_Data->FontSize;
			}
			ImGui::PopClipRect();


			if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
			{
				bIsEditorInputMode = GetCursorPos(&LastCursorPos);
				ShowCursor(!bIsEditorInputMode);
			}

			if (bIsEditorInputMode)
			{
				SetCursorPos(LastCursorPos.x, LastCursorPos.y);
			}

			if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
			{
				ShowCursor(true);
				bIsEditorInputMode = false;
			}

			if (!bIsPlaying && editor_layer->GetSelectedGo() && editor_layer->GetSelectedGo()->GetComponent(engine::Types::kTransformComponent))
			{
				draw_gizmos();
			}
			bInFocus = ImGui::IsWindowFocused();
			
		}


		/*
		for (auto& Message : guid_verbosity_messages)
		{
			auto Color = LogManager::getInstance().GetLevelLogColor(std::get<1>(Message));

			ImGui::TextColored({ Color.x,Color.y, Color.z, Color.w }, std::get<2>(Message).c_str());
		}
		*/
		if(!bIsPlaying)
		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
				ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
				ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

			ImVec2 window_pos = ImGui::GetWindowPos(), window_pos_pivot = {1.f, 0};
			window_pos.y += 50;
			window_pos.x += ImGui::GetWindowSize().x - 10.f;
			ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
			window_flags |= ImGuiWindowFlags_NoMove;
			ImGui::SetNextWindowBgAlpha(0.35f);

			with_Window("Overlay", &bIsPlaying, window_flags)
			{
				const auto RenderTargets = editor_layer->GetApp()->GetEngine()->GetRenderer().GetRenderTargetsList();
				if (ImGui::BeginCombo("###RenderTargetList", SelectedRenderTarget.c_str(),
				                      ImGuiComboFlags_NoArrowButton))
				{
					for (const auto render_target : RenderTargets)
					{
						const bool is_selected = std::strcmp(render_target, SelectedRenderTarget.c_str()) == 0;

						if (ImGui::Selectable(render_target, is_selected))
						{
							SelectedRenderTarget = render_target;
							Texture = editor_layer->GetApp()->GetEngine()->GetRenderer().GetRenderTargetTexture(
								SelectedRenderTarget.c_str()).texture;
						}

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
			}
		}
	}
}

void GameViewWindow::on_resize_viewport(int32 InWidth, int32 InHeight)
{
	Texture = editor_layer->GetApp()->GetEngine()->GetRenderer().GetRenderTargetTexture(SelectedRenderTarget.c_str()).
	                        texture;
}

void GameViewWindow::resize()
{
	if (!FMath::IsNearlyEqual(wsize.x, last_window_size.x) || !FMath::IsNearlyEqual(wsize.y, last_window_size.y))
	{
		last_window_size = wsize;
		editor_layer->GetApp()->GetEngine()->GetRenderer().ResizeViewport(last_window_size.x, last_window_size.y);
		Texture = editor_layer->GetApp()->GetEngine()->GetRenderer().GetRenderTargetTexture(
			SelectedRenderTarget.c_str()).texture;
	}
}

bool GameViewWindow::IsInCameraEditorInputMode() const
{
	return bIsEditorInputMode && !bIsPlaying;
}

void GameViewWindow::SwitchOperationMode()
{
	CurrentOperationMode = CurrentOperationMode == ImGuizmo::WORLD ? ImGuizmo::LOCAL : ImGuizmo::WORLD;
}

bool GameViewWindow::IsPlaying() const
{
	return bIsPlaying;
}

void GameViewWindow::StartPlay()
{
	if (!bIsPlaying)
	{
		EnterGameMode.Broadcast();
		bIsPlaying = true;
		auto& io = ImGui::GetIO();
		io.WantCaptureKeyboard = io.WantCaptureMouse = false;
	}
}

void GameViewWindow::StopPlay()
{
	if (bIsPlaying)
	{
		ExitGameMode.Broadcast();
		bIsPlaying = false;
		auto& io = ImGui::GetIO();
		io.WantCaptureKeyboard = io.WantCaptureMouse = true;
	}
}

void GameViewWindow::OnLogMessageAdded(std::string const& message,loguru::Verbosity verbosity, std::string const& guid)
{
	guid_verbosity_messages.push_back({ guid,verbosity,message });
}

void GameViewWindow::OnLogMessageRemoved(std::string const& guid)
{
	static std::mutex remove_mutex;
	const std::lock_guard<std::mutex> lock(remove_mutex);
	std::erase_if(guid_verbosity_messages, [&guid](std::tuple<std::string, loguru::Verbosity, std::string> const& Value)
		{
			return std::get<0>(Value) == guid;
		});
}

void GameViewWindow::draw_gizmos() const
{
	const auto Editor = static_cast<EditorApplication*>(editor_layer->GetApp());

	ImGuizmo::SetOrthographic(false);
	ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

	const auto transform_component = editor_layer->GetSelectedGo()->GetComponent(engine::Types::kTransformComponent);
	const auto& transform_type = transform_component->GetType();

	auto scale_property = transform_type.GetProperty("LocalScale");
	auto rotation_property = transform_type.GetProperty("Rotation");
	auto position_property = transform_type.GetProperty("Position");

	const auto scale = scale_property.GetValue(*transform_component).value().Unbox<DirectX::SimpleMath::Vector3>();
	const auto rotation = rotation_property.GetValue(*transform_component).value().Unbox<DirectX::SimpleMath::Quaternion>();
	const auto position = position_property.GetValue(*transform_component).value().Unbox<DirectX::SimpleMath::Vector3>();

	auto model = Matrix::Identity;
	model *= Matrix::CreateScale(scale);
	model *= Matrix::CreateFromQuaternion(rotation);
	model *= Matrix::CreateTranslation(position);

	auto isManipulated = Manipulate(
		*Editor->Camera->View.m, 
		*Editor->Camera->Proj.m, 
		CurrentGizmoOperation,	           
		CurrentOperationMode, 
		*model.m);

	if (isManipulated && ImGuizmo::IsUsing())
	{
		DirectX::SimpleMath::Vector3 new_scale;
		DirectX::SimpleMath::Quaternion new_rotation;
		DirectX::SimpleMath::Vector3 new_position;

		model.Decompose(new_scale, new_rotation, new_position);

		if (CurrentGizmoOperation & ImGuizmo::TRANSLATE)
		{
			position_property.SetValue(*transform_component, &new_position);
		}			
		else if (CurrentGizmoOperation & ImGuizmo::ROTATE)
		{
			rotation_property.SetValue(*transform_component, &new_rotation);
		}			
		else if (CurrentGizmoOperation & ImGuizmo::SCALE)
		{			
			scale_property.SetValue(*transform_component, &new_scale);
		}			
		else
		{
			return;
		}			
	}
}
