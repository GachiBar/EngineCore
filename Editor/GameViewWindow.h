#pragma once

#include "imgui/imgui.h"
#include <Windows.h>

class EditorLayer;

class GameViewWindow
{
    void* Texture;
    bool isPlaying = false;
    ImVec2 wsize;


public:
    GameViewWindow(void* Texture);
    void draw_imgui();

    void resize();
    EditorLayer* editor_layer;

    bool IsCameraEditorInputMode() const;

private:
    POINT  LastCursorPos;

    bool bIsEditorInputMode = false;
    ImVec2 last_window_size;
};

