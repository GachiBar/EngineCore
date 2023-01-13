#pragma once

#include "IEditorWindow.h"
#include "EditorCamera.h"
#include "RenderEngine.h"
#include "imgui/imgui.h"
#include "ImGuizmo/ImGuizmo.h"
#include "../Core/Definitions.h"
#include "../Core/libs/Delegates.h"
#include "../GameplaySystem/GameObject.h"
#include "../GameplaySystem/Component.h"

#include <string>
#include <Windows.h>

class GameViewWindow : public IEditorWindow
{
public:
    DECLARE_EVENT(EnterGameModeEvent, GameViewWindow)
    EnterGameModeEvent EnterGameMode;

    DECLARE_EVENT(ExitGameModeEvent, GameViewWindow)
    ExitGameModeEvent ExitGameMode;

    DECLARE_EVENT(ViewportPresentedEvent, GameViewWindow)
    ViewportPresentedEvent ViewportPresented;

    std::shared_ptr<engine::GameObject> GetGameObject();
    void SetGameObject(std::shared_ptr<engine::GameObject> gameObject);

    GameViewWindow(RenderDevice& renderer);

    void Update(float dt);
    void Draw() override;
    void ResizeIfNeed();

    void OnResizeViewport(int32 InWidth, int32 InHeight);
    bool IsPlaying() const;

private:
    std::shared_ptr<engine::GameObject> game_object;
    std::shared_ptr<engine::Component> transform_component;
    EditorCamera editor_camera;
    RenderDevice& renderer;    
    DirectX::SimpleMath::Matrix view;
    DirectX::SimpleMath::Matrix projection;
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

    void UpdateGuizmo();
    void SendCameraData(float dt);
    DirectX::SimpleMath::Matrix GetProjectionMatrix();
    void SwitchOperationMode();
    void DrawMenu();
    void DrawViewport();
    void DrawRenderTargetSelector();
    void DrawGizmo();
    bool IsInCameraEditorInputMode() const;
    bool IsObjectWithTransformSelected() const;

    static bool IsNearlyEqual(ImVec2 lhs, ImVec2 rhs);
};

