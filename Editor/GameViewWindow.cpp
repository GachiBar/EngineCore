#include "GameViewWindow.h"

#include <iostream>

#include "imgui/imgui.h"
#include "libs/MathUtility.h"
#include "EditorLayer.h"

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

    ImGui::BeginChild("GameRender");
    // Get the size of the child (i.e. the whole draw size of the windows).
    ImVec2 wsize = ImGui::GetWindowSize();

    if((FMath::Abs(wsize.x - last_window_size.x) + FMath::Abs(wsize.y - last_window_size.y)) > std::numeric_limits<float>::epsilon())
    {
        last_window_size = ImGui::GetWindowSize();
        editor_layer->GetApp()->GetEngine()->GetRenderer().ResizeViewport(last_window_size.x, last_window_size.y);
    }
   
    // Because I use the texture from OpenGL, I need to invert the V from the UV.


    ImGui::Image(Texture, wsize, ImVec2(0, 0), ImVec2(1, 1));



    ImGui::EndChild();


    windowIsHovered = ImGui::IsItemHovered();

    ImGui::End();
}
