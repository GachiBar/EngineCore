#pragma once

#include "IEditorWindow.h"
#include "EditorCamera.h"
#include "imgui/imgui.h"
#include "ImGuizmo/ImGuizmo.h"
#include "../Core/Definitions.h"
#include "../Core/libs/Delegates.h"
#include "../GameplaySystem/GameObject.h"
#include "../GameplaySystem/Component.h"

#include <string>
#include <Windows.h>

class EditorLayer;

class GameViewWindow : public IEditorWindow
{
public:
    DECLARE_EVENT(EnterGameModeEvent, GameViewWindow)
    EnterGameModeEvent EnterGameMode;

    DECLARE_EVENT(ExitGameModeEvent, GameViewWindow)
    ExitGameModeEvent ExitGameMode;

    std::shared_ptr<engine::GameObject> GetGameObject();
    void SetGameObject(std::shared_ptr<engine::GameObject> gameObject);

    GameViewWindow(EditorLayer* editorLayer, EditorCamera& editorCamera);

    void Update();
    void Draw() override;
    void ResizeIfNeed();

    void on_resize_viewport(int32 InWidth,int32 InHeight);      

    bool IsInCameraEditorInputMode() const;    

    bool IsPlaying() const;

private:
    std::shared_ptr<engine::GameObject> game_object;
    std::shared_ptr<engine::Component> transform_component;
    EditorLayer* editor_layer;
    RenderDevice& renderer;
    EditorCamera& editor_camera;
    std::string selected_render_target;
    void* texture;
    bool is_playing;
    bool is_in_focus;
    bool is_camera_input_mode;
    ImVec2 previous_window_size;
    ImVec2 current_window_size;
    POINT last_cursor_position;

    ImGuizmo::MODE current_operation_mode = ImGuizmo::LOCAL;
    ImGuizmo::OPERATION current_gizmo_operation = ImGuizmo::OPERATION::TRANSLATE;

    void SwitchOperationMode();
    void DrawMenu();
    void DrawViewport();
    void DrawRenderTargetSelector();
    void DrawGizmo();
    bool IsObjectWithTransformSelected();

    static bool IsNearlyEqual(ImVec2 lhs, ImVec2 rhs);
};

