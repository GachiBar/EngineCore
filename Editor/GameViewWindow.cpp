#include "GameViewWindow.h"

#include <iostream>

#include "imgui/imgui.h"
#include "libs/MathUtility.h"
#include "EditorLayer.h"
#include "libs/imgui_sugar.hpp"

GameViewWindow::GameViewWindow(void* InTexture, EditorLayer* InEditorLayer): Texture(InTexture),
                                                                             editor_layer(InEditorLayer)
{
	SelectedRenderTarget = *editor_layer->GetApp()->GetEngine()->GetRenderer().GetRenderTargetsList().rbegin();
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
				//EventSystem.notify(null, new Event(EventType.GameEngineStartPlay));
			}
			if (ImGui::MenuItem("Stop", "", !isPlaying, isPlaying))
			{
				isPlaying = false;
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

bool GameViewWindow::IsCameraEditorInputMode() const
{
	return bIsEditorInputMode;
}
