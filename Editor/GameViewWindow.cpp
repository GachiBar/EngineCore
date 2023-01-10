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

GameViewWindow::GameViewWindow(EditorLayer* editorLayer, EditorCamera& editorCamera)
	: editor_camera(editorCamera)
	, selected_render_target("outTexture")
	, texture(nullptr)
	, is_playing(false)
	, is_in_focus(false)
	, editor_layer(editorLayer)
{}

void GameViewWindow::Update()
{
	if(is_in_focus && !IsInCameraEditorInputMode())
	{
		const auto input = InputManager::getInstance().player_input;

		if (input->WasJustPressed(EKeys::W)) 
		{
			current_gizmo_operation = ImGuizmo::TRANSLATE;
		}			
		if (input->WasJustPressed(EKeys::E))
		{
			current_gizmo_operation = ImGuizmo::ROTATE;
		}			
		if (input->WasJustPressed(EKeys::R)) 
		{
			current_gizmo_operation = ImGuizmo::SCALE;
		}			
		if (input->WasJustPressed(EKeys::C)) 
		{
			SwitchOperationMode();
		}			
	}
}

static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs)            
{ 
	return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); 
}

void GameViewWindow::Draw()
{
	with_Window("Game Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse
		| ImGuiWindowFlags_MenuBar)
	{
		wsize = ImGui::GetWindowSize();
		with_MenuBar
		{
			if (ImGui::MenuItem("Play", "", is_playing, !is_playing))
			{
				editor_layer->GetApp()->GetEngine()->GetScene()->Initialize();
				StartPlay();
			}
			if (ImGui::MenuItem("Stop", "", !is_playing, is_playing))
			{
				editor_layer->GetApp()->GetEngine()->GetScene()->Terminate();
				StopPlay();
			}
		}

		with_Child("GameRender", {}, false,  ImGuiWindowFlags_NoBackground)
		{
			{
				if (texture == nullptr)
					texture = editor_layer->GetApp()->GetEngine()->GetRenderer().GetRenderTargetTexture(
						selected_render_target.c_str()).texture;

				if (is_playing) {
					ImGui::GetWindowDrawList()->AddImage(texture, ImGui::GetWindowPos(), ImGui::GetWindowPos() + ImGui::GetWindowSize());
					editor_layer->GetApp()->DrawGameUI();
				}
				else {
					ImGui::Image(texture, ImGui::GetWindowSize());
				}
			}

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

			if (!is_playing && editor_layer->GetSelectedGo() && editor_layer->GetSelectedGo()->GetComponent(engine::Types::kTransformComponent))
			{
				DrawGizmo();
			}
			is_in_focus = ImGui::IsWindowFocused();
			
		}

		if(!is_playing)
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

			with_Window("Overlay", &is_playing, window_flags)
			{
				const auto RenderTargets = editor_layer->GetApp()->GetEngine()->GetRenderer().GetRenderTargetsList();
				if (ImGui::BeginCombo("###RenderTargetList", selected_render_target.c_str(),
				                      ImGuiComboFlags_NoArrowButton))
				{
					for (const auto render_target : RenderTargets)
					{
						const bool is_selected = std::strcmp(render_target, selected_render_target.c_str()) == 0;

						if (ImGui::Selectable(render_target, is_selected))
						{
							selected_render_target = render_target;
							texture = editor_layer->GetApp()->GetEngine()->GetRenderer().GetRenderTargetTexture(
								selected_render_target.c_str()).texture;
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
	auto& renderer = editor_layer->GetApp()->GetEngine()->GetRenderer();
	texture = renderer.GetRenderTargetTexture(selected_render_target.c_str()).texture;
}

void GameViewWindow::resize()
{
	if (!FMath::IsNearlyEqual(wsize.x, last_window_size.x) || !FMath::IsNearlyEqual(wsize.y, last_window_size.y))
	{
		last_window_size = wsize;
		editor_layer->GetApp()->GetEngine()->GetRenderer().ResizeViewport(last_window_size.x, last_window_size.y);
		texture = editor_layer->GetApp()->GetEngine()->GetRenderer().GetRenderTargetTexture(
			selected_render_target.c_str()).texture;
	}
}

bool GameViewWindow::IsInCameraEditorInputMode() const
{
	return bIsEditorInputMode && !is_playing;
}

void GameViewWindow::SwitchOperationMode()
{
	current_operation_mode = current_operation_mode == ImGuizmo::WORLD ? ImGuizmo::LOCAL : ImGuizmo::WORLD;
}

bool GameViewWindow::IsPlaying() const
{
	return is_playing;
}

void GameViewWindow::StartPlay()
{
	if (!is_playing)
	{
		EnterGameMode.Broadcast();
		is_playing = true;
		auto& io = ImGui::GetIO();
		io.WantCaptureKeyboard = io.WantCaptureMouse = false;
	}
}

void GameViewWindow::StopPlay()
{
	if (is_playing)
	{
		ExitGameMode.Broadcast();
		is_playing = false;
		auto& io = ImGui::GetIO();
		io.WantCaptureKeyboard = io.WantCaptureMouse = true;
	}
}

void GameViewWindow::DrawGizmo()
{
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
		*editor_camera.View.m,
		*editor_camera.Proj.m,
		current_gizmo_operation,
		current_operation_mode,
		*model.m);

	if (isManipulated && ImGuizmo::IsUsing())
	{
		DirectX::SimpleMath::Vector3 new_scale;
		DirectX::SimpleMath::Quaternion new_rotation;
		DirectX::SimpleMath::Vector3 new_position;

		model.Decompose(new_scale, new_rotation, new_position);

		if (current_gizmo_operation & ImGuizmo::TRANSLATE)
		{
			position_property.SetValue(*transform_component, &new_position);
		}			
		else if (current_gizmo_operation & ImGuizmo::ROTATE)
		{
			rotation_property.SetValue(*transform_component, &new_rotation);
		}			
		else if (current_gizmo_operation & ImGuizmo::SCALE)
		{			
			scale_property.SetValue(*transform_component, &new_scale);
		}			
		else
		{
			return;
		}			
	}
}
