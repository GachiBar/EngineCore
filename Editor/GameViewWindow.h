#pragma once

#include <string>

#include "imgui/imgui.h"
#include <Windows.h>

#include "Definitions.h"

class EditorLayer;

class GameViewWindow
{
    void* Texture;
    bool isPlaying = false;
    ImVec2 wsize;


public:
    GameViewWindow(void* InTexture, EditorLayer* InEditorLayer);
    void draw_imgui();

    void on_resize_viewport(int32 InWidth,int32 InHeight);
    void resize();
    EditorLayer* editor_layer;

    bool IsCameraEditorInputMode() const;

private:
    std::string SelectedRenderTarget;
    POINT  LastCursorPos;

    bool bIsEditorInputMode = false;
    ImVec2 last_window_size;
};

