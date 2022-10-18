#include "MenuBar.h"

#include "imgui/imgui.h"

void MenuBar::draw_imgui()
{
    ImGui::BeginMenuBar();

    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Save", "Ctrl+S")) {
            //EventSystem.notify(null, new Event(EventType.SaveLevel));
        }

        if (ImGui::MenuItem("Load", "Ctrl+O")) {
            //EventSystem.notify(null, new Event(EventType.LoadLevel));
        }

        ImGui::EndMenu();
    }

    ImGui::EndMenuBar();
}
