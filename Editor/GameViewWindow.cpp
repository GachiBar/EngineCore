#include "GameViewWindow.h"

#include "EditorApplication.h"
#include "imgui/imgui.h"
#include "libs/MathUtility.h"
#include "EditorLayer.h"
#include "InputManager.h"
#include "ImGuizmo/ImGuizmo.h"
#include "libs/imgui_sugar.hpp"
#include "../GameplaySystem/Component.h"

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

void GameViewWindow::draw_imgui()
{
	with_Window("Game Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse
	            | ImGuiWindowFlags_MenuBar)
	{
		with_MenuBar
		{
			if (ImGui::MenuItem("Play", "", isPlaying, !isPlaying))
			{
				isPlaying = true;

				EditorApplication* Editor = static_cast<EditorApplication*>(editor_layer->GetApp());
				if (Editor)
					Editor->SetGameOnlyOnlyInputMode();
				//EventSystem.notify(null, new Event(EventType.GameEngineStartPlay));
			}
			if (ImGui::MenuItem("Stop", "", !isPlaying, isPlaying))
			{
				isPlaying = false;
				EditorApplication* Editor = static_cast<EditorApplication*>(editor_layer->GetApp());
				if (Editor)
					Editor->SetEditorOnlyInputMode();
				//EventSystem.notify(null, new Event(EventType.GameEngineStopPlay));
			}
		}

		with_Child("GameRender")
		{
			// Get the size of the child (i.e. the whole draw size of the windows).

			// Because I use the texture from OpenGL, I need to invert the V from the UV.
			wsize = ImGui::GetWindowSize();

			ImGui::Image(Texture, wsize, ImVec2(0, 0), ImVec2(1, 1));

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

			if (!isPlaying && editor_layer->GetSelectedGo())
			{
				draw_gizmos();
			}
			bInFocus = ImGui::IsWindowFocused();
		}

		if(!isPlaying)
		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

			ImVec2 window_pos = ImGui::GetWindowPos(), window_pos_pivot = {1.f,0};
			window_pos.y += 50;
			window_pos.x += ImGui::GetWindowSize().x - 10.f;
			ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
			window_flags |= ImGuiWindowFlags_NoMove;
			ImGui::SetNextWindowBgAlpha(0.35f);

			with_Window("Overlay", &isPlaying, window_flags)
			{
				const auto RenderTargets = editor_layer->GetApp()->GetEngine()->GetRenderer().GetRenderTargetsList();
				if (ImGui::BeginCombo("###RenderTargetList", SelectedRenderTarget.c_str(), ImGuiComboFlags_NoArrowButton))
				{
					for (const auto render_target : RenderTargets)
					{
						const bool is_selected = std::strcmp(render_target, SelectedRenderTarget.c_str()) == 0;

						if (ImGui::Selectable(render_target, is_selected))
						{
							SelectedRenderTarget = render_target;
							Texture = editor_layer->GetApp()->GetEngine()->GetRenderer().GetRenderTargetTexture(SelectedRenderTarget.c_str()).texture;
							//editor_layer->GetApp()->GetEngine()->GetRenderer().
						}
							

						// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
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
	Texture = editor_layer->GetApp()->GetEngine()->GetRenderer().GetRenderTargetTexture(SelectedRenderTarget.c_str()).texture;
}

void GameViewWindow::resize()
{
	if (!FMath::IsNearlyEqual(wsize.x, last_window_size.x) || !FMath::IsNearlyEqual(wsize.y, last_window_size.y))
	{
		last_window_size = wsize;
		editor_layer->GetApp()->GetEngine()->GetRenderer().ResizeViewport(last_window_size.x, last_window_size.y);
		Texture = editor_layer->GetApp()->GetEngine()->GetRenderer().GetRenderTargetTexture(SelectedRenderTarget.c_str()).texture;
	}
}

bool GameViewWindow::IsInCameraEditorInputMode() const
{
	return bIsEditorInputMode && !isPlaying;
}

void GameViewWindow::SwitchOperationMode()
{
	CurrentOperationMode = CurrentOperationMode == ImGuizmo::WORLD ? ImGuizmo::LOCAL : ImGuizmo::WORLD;
}

void GameViewWindow::draw_gizmos()
{
	auto Editor = static_cast<EditorApplication*>(editor_layer->GetApp());

	if (!isPlaying && editor_layer->GetSelectedGo())
	{
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();
		float windowWidth = (float)ImGui::GetWindowWidth();
		float windowHeight = (float)ImGui::GetWindowHeight();

		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

		auto change_mat = [](const DirectX::XMMATRIX mat)
		{
			DirectX::XMFLOAT4X4 temp{};
			XMStoreFloat4x4(&temp, mat);
			return temp;
		};

		auto value = static_cast<float*>(editor_layer->GetSelectedGo()->GetComponent("GameplayCore.Components", "TransformComponent")->GetProperty("ModelMatrix").GetValue().value().Unbox());

		DirectX::XMFLOAT4X4 v = change_mat(Editor->Camera->View);
		DirectX::XMFLOAT4X4 p = change_mat(Editor->Camera->Proj);
		DirectX::XMFLOAT4X4 w = DirectX::XMFLOAT4X4(value);

		DirectX::XMFLOAT4X4 delta = {};
		ImGuizmo::Manipulate(&v.m[0][0], &p.m[0][0], CurrentGizmoOperation, CurrentOperationMode, &w.m[0][0]);

		if(ImGuizmo::IsUsing())
		{
			float trans[3];
			float rot[3];
			float scale[3];

			ImGuizmo::DecomposeMatrixToComponents(&w.m[0][0], trans, rot, scale);

			auto rot_quat = DirectX::SimpleMath::Quaternion::CreateFromRotationMatrix(w); //DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(rot[0], rot[1], rot[2]);
			float rot_quat_representaion[4] = { rot_quat.x,rot_quat.y,rot_quat.z ,rot_quat.w, };

			switch (CurrentGizmoOperation) {

			case ImGuizmo::TRANSLATE_X:
			case ImGuizmo::TRANSLATE_Y:
			case ImGuizmo::TRANSLATE_Z:
			case ImGuizmo::TRANSLATE:
				editor_layer->GetSelectedGo()->GetComponent("GameplayCore.Components", "TransformComponent")->GetProperty("LocalPosition").SetValue(trans);
				break;

			case ImGuizmo::ROTATE_X:
			case ImGuizmo::ROTATE_Y:
			case ImGuizmo::ROTATE_Z:
			case ImGuizmo::ROTATE_SCREEN:
			case ImGuizmo::ROTATE:
				editor_layer->GetSelectedGo()->GetComponent("GameplayCore.Components", "TransformComponent")->GetProperty("LocalRotation").SetValue(rot_quat_representaion);
				break;

			case ImGuizmo::SCALE_X:
			case ImGuizmo::SCALE_Y:
			case ImGuizmo::SCALE_Z:
			case ImGuizmo::SCALE:
				editor_layer->GetSelectedGo()->GetComponent("GameplayCore.Components", "TransformComponent")->GetProperty("LocalScale").SetValue(scale);
				break;

			default: ;
			}
			
			
		}
	}
}
