#pragma once


class GameViewWindow
{
    bool isPlaying = false;
    bool windowIsHovered;
public:
    void draw_imgui();

    bool getWantCaptureMouse();
};

