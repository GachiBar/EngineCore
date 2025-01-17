#include "EditorLayer.h"
#include "Application.h"
#include "LayerStack.h"
#include "AIEditorWindow.h"
#include "Resources/ExplorerWindow.h"
#include "EditorApplication.h"
#include "InputManager.h"
#include "PropertyWindow.h"
#include "AIEditorWindow.h"
#include "../GameplaySystem/Component.h"
#include "ImGuizmo/ImGuizmo.h"
#include "libs/imgui_sugar.hpp"
#include "imgui/imgui.h"
#include "Resources/MaterialsEditor.h"
#include "winApiFileLoad.h"
#include "FilePathsHelper.h"

#include <fstream>

namespace Renderer
{
	class D3D11Renderer;
}

const std::wstring& EditorLayer::GetScenePath()
{
    return scene_path;
}

void EditorLayer::SetScenePath(std::wstring path)
{
    scene_path = std::move(path);
    hierarchy->SetSceneName(GetSceneName(scene_path));
}

EditorLayer::EditorLayer(LayerStack* owner)
    : Layer(owner, "EditorLayer")
    , selected_go(nullptr)
{
    CurrentInputMode = EEditorInputMode::Type::EditorOnlyMode;
}

void EditorLayer::OnAttach()
{
	gvm = std::make_shared<GameViewWindow>(GetApp()->GetEngine()->GetRenderer());

	hierarchy = std::make_shared<SceneHierarchyWindow>();
    game_object_inspector = std::make_shared<GameObjectInspectorWindow>();
    resourceDrawer = std::make_shared<MaterialsEditor>();
    ai_editor = std::make_shared<AIEditorWindow>();
    property_window = std::make_shared<PropertyWindow>(game_object_inspector, resourceDrawer, ai_editor);
    SettingsWindow = std::make_shared<ProjectSettingsWindow>();
    explorer = std::make_shared<ExplorerWindow>(GetApp());
    log = std::make_shared<LogWindow>();

	hierarchy.get()->GameObjectSelected.AddLambda([&](std::shared_ptr<engine::GameObject> go)
	{
		selected_go = go;
        gvm->SetGameObject(go);
	    property_window->SelectGameObject(go);
	});

    gvm.get()->GameObjectSelected.AddLambda([&](std::shared_ptr<engine::GameObject> go)
    {
        hierarchy->SetSelected(go);
        property_window->SelectGameObject(go);
    });

    explorer.get()->FileSelected.AddLambda([&](const std::filesystem::path& path)
    {
        property_window->SelectResource(path);
    });

    hierarchy->SetScene(GetApp()->GetEngine()->GetScene());
    game_object_inspector->SetScene(GetApp()->GetEngine()->GetScene());
    gvm->SetScene(GetApp()->GetEngine()->GetScene());

	auto& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void EditorLayer::OnDetach()
{
	
}

void EditorLayer::OnUpdate(float const dt)
{
	Layer::OnUpdate(dt);
    gvm->Update(dt);
}

void EditorLayer::OnGuiRender()
{
    ImGuizmo::BeginFrame();

    bool p_open = true;
    //ImGui::ShowDemoWindow(&p_open);

    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    
    
    ImGui::Begin("DockSpace Demo", &p_open, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    else
    {
        //ShowDockingDisabledMessage();
    }
    ImGui::BeginDisabled(gvm->IsPlaying());

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "Ctrl+N"))
            {
                std::wstring copyPath;

                if (SaveSceneAs(copyPath))
                {                
                    scene_path = copyPath;
                    DeserializeScene(scene_path);
                }
            }

            if (ImGui::MenuItem("Open...", "Ctrl+O"))
            {
                if (LoadScene())
                {
                    game_object_inspector->SetGameObject(nullptr);
                    gvm->SetGameObject(nullptr);
                    auto sceneName = GetSceneName(scene_path);
                    hierarchy->SetSceneName(sceneName);
                }
            }

            if (ImGui::MenuItem("Save", "Ctrl+S"))
            {
                if (SaveScene())
                {
                    auto sceneName = GetSceneName(scene_path);
                    hierarchy->SetSceneName(sceneName);
                }
            }

            if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
            {
                std::wstring copyPath;
                SaveSceneAs(copyPath);
            }

            if (ImGui::MenuItem("Exit"))
            {
                GetApp()->Close();
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding", NULL, &opt_padding);

            if (ImGui::BeginMenu("Docking options"))
            {
                if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
                if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
                if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
                if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
                if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Windows"))
            {
                if (ImGui::MenuItem("Log", NULL, &log->bIsOpen))
                    log->bIsOpen != log->bIsOpen;

                ImGui::EndMenu();
            }

            if (ImGui::MenuItem("Close", NULL, false, &p_open != NULL))
                p_open = false;

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::EndDisabled();

    ImGui::End();

    gvm->Draw();
    hierarchy->Draw();
    property_window->Draw();

    ImGui::BeginDisabled(gvm->IsPlaying());

    SettingsWindow->Draw();
    explorer->Draw();
    ImGui::EndDisabled();

    if(log->bIsOpen)
		log->Draw();
}

void EditorLayer::OnPostRender()
{
    gvm->ResizeIfNeed(); 
    Layer::OnPostRender();
}

engine::GameObject* EditorLayer::GetSelectedGo()
{
    return selected_go.get();
}

void EditorLayer::SerializeScene(std::wstring path)
{
    std::string json = GetApp()->GetEngine()->GetScene()->Serialize();
    std::ofstream file_handler(path, std::ios::out | std::ios::trunc);
    file_handler << json;
}

void EditorLayer::DeserializeScene(std::wstring path)
{
    std::fstream file(path, std::fstream::in);
    std::string json;
    getline(file, json, '\0');
    GetApp()->GetEngine()->GetScene()->Deserialize(json);
}

bool EditorLayer::LoadScene()
{
    auto scenes_names = LoadFileFromExplorer(L"Scenes", L"Scene|*.dat");

    if (scenes_names.empty()) 
    {
        return false;
    }

    scene_path = scenes_names[0];
    DeserializeScene(scene_path);
    return true;
}

bool EditorLayer::SaveScene()
{
    if (scene_path.empty())
    {
        auto scenes_names = SaveFileToExplorer(L"Scenes", L"Default.dat", L"Scene|*.dat");
        
        if (scenes_names.empty())
        {
            return false;
        }
            
        scene_path = scenes_names[0];
    }

    SerializeScene(scene_path);
    return true;
}

bool EditorLayer::SaveSceneAs(std::wstring& copyPath_out)
{  
    auto scenes_names = SaveFileToExplorer(L"Scenes", L"Default.dat", L"Scene|*.dat");
    
    if (scenes_names.empty())
    {
        return false;
    }
       
    copyPath_out = scenes_names[0];
    SerializeScene(copyPath_out);
    return true;
}

std::string EditorLayer::GetSceneName(std::wstring scene_path)
{
    auto name = FilePahtsHelper::RemoveExtension(FilePahtsHelper::BaseName(scene_path));
    return FilePahtsHelper::ws2s(name);
}