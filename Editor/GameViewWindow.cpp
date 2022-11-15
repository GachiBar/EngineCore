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

GameViewWindow::GameViewWindow(void* InTexture, EditorLayer* InEditorLayer): Texture(InTexture),
                                                                             editor_layer(InEditorLayer)
{
	SelectedRenderTarget = *editor_layer->GetApp()->GetEngine()->GetRenderer().GetRenderTargetsList().rbegin();
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

void GameViewWindow::Draw()
{
	with_Window("Game Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse
	            | ImGuiWindowFlags_MenuBar)
	{
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

		with_Child("GameRender")
		{
			ImGui::Image(Texture, ImGui::GetWindowSize(), ImVec2(0, 0), ImVec2(1, 1));

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

			if (!bIsPlaying && editor_layer->GetSelectedGo() && editor_layer->GetSelectedGo()->GetComponent(
				"GameplayCore.Components", "TransformComponent"))
			{
				draw_gizmos();
			}
			bInFocus = ImGui::IsWindowFocused();
		}

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
		EnteringGameMode.Broadcast();
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

std::optional<std::string> GameViewWindow::CurrentOperationToString() const
{
	if (CurrentGizmoOperation & ImGuizmo::TRANSLATE)
	{
		return "Position";
	}
	if (CurrentGizmoOperation & ImGuizmo::ROTATE)
	{
		return "Rotation";
	}
	if (CurrentGizmoOperation & ImGuizmo::SCALE)
	{
		return "Scale";
	}
	return std::nullopt;
}

void GameViewWindow::draw_gizmos() const
{
	const auto Editor = static_cast<EditorApplication*>(editor_layer->GetApp());

	ImGuizmo::SetOrthographic(false);
	ImGuizmo::SetDrawlist();

	ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

	const auto transform_component = editor_layer->GetSelectedGo()->GetComponent("GameplayCore.Components", "TransformComponent");

	auto value = static_cast<float*>(transform_component->GetProperty("ModelMatrix").GetValue().value().Unbox());
	auto w = DirectX::XMFLOAT4X4(value);

	Manipulate(&Editor->Camera->View.m[0][0], &Editor->Camera->Proj.m[0][0], CurrentGizmoOperation,
	           CurrentOperationMode, &w.m[0][0]);

	if (ImGuizmo::IsUsing())
	{
		float trans[3];
		float rot[3];
		float scale[3];

		ImGuizmo::DecomposeMatrixToComponents(&w.m[0][0], trans, rot, scale);

		const auto rot_quat = DirectX::SimpleMath::Quaternion::CreateFromRotationMatrix(w);
		float rot_quat_representaion[4] = {rot_quat.x, rot_quat.y, rot_quat.z, rot_quat.w,};

		const std::string property_name = CurrentOperationToString().value_or("Position");

		void* res_data;
		if (CurrentGizmoOperation & ImGuizmo::TRANSLATE)
			res_data = trans;
		else if (CurrentGizmoOperation & ImGuizmo::ROTATE)
			res_data = rot_quat_representaion;
		else if (CurrentGizmoOperation & ImGuizmo::SCALE)
			res_data = scale;
		else
			return;

		transform_component->GetProperty(property_name).SetValue(res_data);
	}
}
