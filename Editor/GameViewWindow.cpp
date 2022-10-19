#include "GameViewWindow.h"
//#include "imgui/imgui.h"
#include "imgui/imgui_sugar.hpp"

void GameViewWindow::draw_imgui()
{
    with_Window("Game Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse| ImGuiWindowFlags_MenuBar)
    {
        with_MenuBar
        {
             if (ImGui::MenuItem("Play", "", isPlaying, !isPlaying)) {
				isPlaying = true;
            //EventSystem.notify(null, new Event(EventType.GameEngineStartPlay));
             }
	        if (ImGui::MenuItem("Stop", "", !isPlaying, isPlaying)) {
	            isPlaying = false;
	            //EventSystem.notify(null, new Event(EventType.GameEngineStopPlay));
	        }
        }
        with_Child("GameRender")
        {
            // Get the size of the child (i.e. the whole draw size of the windows).
            ImVec2 wsize = ImGui::GetWindowSize();
            //ImGui::Image((ImTextureID)tex, wsize, ImVec2(0, 1), ImVec2(1, 0));
        }

        windowIsHovered = ImGui::IsItemHovered();
    }
    /*
    ImGui::Begin("Game Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse
        | ImGuiWindowFlags_MenuBar);

   

    ImGui::End();
    */
}

bool GameViewWindow::getWantCaptureMouse()
{
	return windowIsHovered;
}
