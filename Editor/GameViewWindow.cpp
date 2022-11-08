#include "GameViewWindow.h"

#include <iostream>

#include "imgui/imgui.h"
#include "libs/MathUtility.h"
#include "EditorLayer.h"
#include "libs/imgui_sugar.hpp"

GameViewWindow::GameViewWindow(void* Texture):Texture(Texture)
{
}

void GameViewWindow::draw_imgui()
{

    ImGui::Begin("Game Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse
        | ImGuiWindowFlags_MenuBar);

    ImGui::BeginMenuBar();
    if (ImGui::MenuItem("Play", "", isPlaying, !isPlaying)) {
        isPlaying = true;
        //EventSystem.notify(null, new Event(EventType.GameEngineStartPlay));
    }
    if (ImGui::MenuItem("Stop", "", !isPlaying, isPlaying)) {
        isPlaying = false;
        //EventSystem.notify(null, new Event(EventType.GameEngineStopPlay));
    }
    ImGui::EndMenuBar();


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

    ImGui::End();
}

void GameViewWindow::resize()
{
    if((FMath::Abs(wsize.x - last_window_size.x) + FMath::Abs(wsize.y - last_window_size.y)) > std::numeric_limits<float>::epsilon())
    {
        last_window_size = wsize;
        editor_layer->GetApp()->GetEngine()->GetRenderer().ResizeViewport(last_window_size.x, last_window_size.y);
        Texture = editor_layer->GetApp()->GetEngine()->GetRenderer().GetRenderTargetTexture("outTexture").texture;
    }
}

bool GameViewWindow::IsCameraEditorInputMode() const
{
    return bIsEditorInputMode;
}
