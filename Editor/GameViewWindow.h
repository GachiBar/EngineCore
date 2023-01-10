#pragma once

#include "IEditorWindow.h"
#include "EditorCamera.h"
#include "imgui/imgui.h"
#include "ImGuizmo/ImGuizmo.h"
#include "../Core/Definitions.h"
#include "../Core/libs/Delegates.h"
#include "../Core/libs/loguru/loguru.hpp"

#include <string>
#include <Windows.h>

class EditorLayer;

class GameViewWindow : public IEditorWindow
{
public:
	
    GameViewWindow(EditorLayer* editorLayer, EditorCamera& editorCamera);

    void Update();
    void Draw() override;

    void on_resize_viewport(int32 InWidth,int32 InHeight);
    void resize();    

    bool IsInCameraEditorInputMode() const;

    void SwitchOperationMode();

    DECLARE_EVENT(EnterGameModeEvent, GameViewWindow)
    EnterGameModeEvent EnterGameMode;

    DECLARE_EVENT(ExitGameModeEvent, GameViewWindow)
	ExitGameModeEvent ExitGameMode;

    bool IsPlaying() const;

private:
    void StartPlay();
    void StopPlay();
    void DrawGizmo();

    EditorLayer* editor_layer;
    EditorCamera& editor_camera;
    std::string selected_render_target;
    void* texture;
    bool is_playing;
    bool is_in_focus;
    ImVec2 wsize;    

    ImGuizmo::MODE current_operation_mode = ImGuizmo::LOCAL;
    ImGuizmo::OPERATION current_gizmo_operation = ImGuizmo::OPERATION::TRANSLATE;   
    
    POINT  LastCursorPos{};

    bool bIsEditorInputMode = false;
    ImVec2 last_window_size;
};

