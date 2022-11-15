#pragma once

#include <optional>
#include <string>

#include "imgui/imgui.h"
#include <Windows.h>

#include "Definitions.h"
#include "ImGuizmo/ImGuizmo.h"
#include "libs/Delegates.h"

class EditorLayer;

class GameViewWindow
{
public:
    GameViewWindow(void* InTexture, EditorLayer* InEditorLayer);

    void update();

    void draw_imgui();

    void on_resize_viewport(int32 InWidth,int32 InHeight);
    void resize();
    EditorLayer* editor_layer;

    bool IsInCameraEditorInputMode() const;

    void SwitchOperationMode();

    DECLARE_EVENT(EnterToPlayFromEditorEvent, GameViewWindow)
	EnterToPlayFromEditorEvent EnteringGameMode;

    DECLARE_EVENT(ExitGameModeEvent, GameViewWindow)
	ExitGameModeEvent ExitGameMode;

    bool IsPlaying() const;

    void StartPlay();
    void StopPlay();
private:
    void* Texture;
    bool bIsPlaying = false;
    ImVec2 wsize;

    std::optional<std::string> CurrentOperationToString() const;
    bool bInFocus = false;

    ImGuizmo::MODE CurrentOperationMode = ImGuizmo::WORLD;
    ImGuizmo::OPERATION CurrentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;

    void draw_gizmos() const;
    std::string SelectedRenderTarget;
    POINT  LastCursorPos{};

    bool bIsEditorInputMode = false;
    ImVec2 last_window_size;
};

