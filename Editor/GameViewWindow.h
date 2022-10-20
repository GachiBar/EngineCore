#pragma once


class GameViewWindow
{
    void* Texture;
    bool isPlaying = false;
    bool windowIsHovered;
public:
    GameViewWindow(void* Texture);
    void draw_imgui();

    bool getWantCaptureMouse() {
        return windowIsHovered;
    }

private:
    /*
    ImVec2 getLargestSizeForViewport() {
        ImVec2 windowSize = ImGui::GetContentRegionAvail();

        float aspectWidth = windowSize.x;
        float aspectHeight = aspectWidth / (16.0f / 9.0f);
        if (aspectHeight > windowSize.y) {
            // We must switch to pillarbox mode
            aspectHeight = windowSize.y;
            aspectWidth = aspectHeight * (16.0f / 9.0f);
        }

        return {aspectWidth, aspectHeight};
    }

    ImVec2 getCenteredPositionForViewport(ImVec2 aspectSize) {
        ImVec2 windowSize=ImGui::GetContentRegionAvail();

        float viewportX = (windowSize.x / 2.0f) - (aspectSize.x / 2.0f);
        float viewportY = (windowSize.y / 2.0f) - (aspectSize.y / 2.0f);

        return {viewportX + ImGui::GetCursorPosX(), viewportY + ImGui::GetCursorPosY()};
    }
    */
};

