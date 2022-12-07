#pragma once

#include <optional>
#include <string>
#include <tuple>

#include "imgui/imgui.h"
#include <Windows.h>

#include "IEditorWindow.h"
#include "ImGuizmo/ImGuizmo.h"
#include "../Core/Definitions.h"
#include "../Core/libs/Delegates.h"
#include "../Core/libs/loguru/loguru.hpp"

class EditorLayer;

class GameViewWindow : public IEditorWindow
{
public:
	
    GameViewWindow(void* InTexture, EditorLayer* InEditorLayer);

    void update();

    void Draw() override;

    void on_resize_viewport(int32 InWidth,int32 InHeight);
    void resize();
    EditorLayer* editor_layer;

    bool IsInCameraEditorInputMode() const;

    void SwitchOperationMode();

    DECLARE_EVENT(EnterGameModeEvent, GameViewWindow)
    EnterGameModeEvent EnterGameMode;

    DECLARE_EVENT(ExitGameModeEvent, GameViewWindow)
	ExitGameModeEvent ExitGameMode;

    bool IsPlaying() const;

    void StartPlay();
    void StopPlay();

private:
    void OnLogMessageAdded(std::string const& message, loguru::Verbosity verbosity, std::string const& guid);
    void OnLogMessageRemoved(std::string const& guid);
private:
    std::vector<std::tuple<std::string, loguru::Verbosity, std::string>> guid_verbosity_messages;

    void* Texture;
    bool bIsPlaying = false;
    ImVec2 wsize;

    bool bInFocus = false;

    ImGuizmo::MODE CurrentOperationMode = ImGuizmo::LOCAL;
    ImGuizmo::OPERATION CurrentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;

    void draw_gizmos() const;
    std::string SelectedRenderTarget;
    POINT  LastCursorPos{};

    bool bIsEditorInputMode = false;
    ImVec2 last_window_size;
};

