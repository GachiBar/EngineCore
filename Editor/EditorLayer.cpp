#include "EditorLayer.h"

//#include <imgui/imgui.h>

#include "Application.h"
#include "LayerStack.h"
//#include "../../FNADx11RenderDevice/include/D3D11Renderer.h"
//#include "../../DX11RenderEngine/DX11RenderEngine/source/CoreRenderSystem/Graphics.h"
#include <iostream>

#include "ImGuizmo/ImGuizmo.h"
#include "libs/nameof.hpp"
#include "InputSystem/ConfigReaderWriterFactory.h"
//#include "imgui/backends/imgui_impl_win32.h"
//#include "imgui/backends/imgui_impl_dx11.h"

namespace Renderer
{
	class D3D11Renderer;
}

EditorLayer::EditorLayer(LayerStack* owner) : Layer(owner, "EditorLayer"), selected_go(nullptr)
{
}

void EditorLayer::OnAttach()
{
	gvm = std::make_shared<GameViewWindow>(GetApp()->GetEngine()->GetRenderer().GetRenderTargetTexture("outTexture").texture);
	hierarchy = std::make_shared<SceneHierarchyWindow>();
    properties = std::make_shared<PropertyWindow>(GetApp()->GetAssembly());
    SettingsWindow = std::make_shared<ProjectSettingsWindow>();

	hierarchy.get()->OnSelectGameObjectInHierarchy.BindLambda([&](std::shared_ptr<engine::GameObject>& go)
	{
		selected_go = go;
	});

	auto& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void EditorLayer::OnDetach()
{
	
}

void EditorLayer::OnGuiRender()
{
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

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows, 
            // which we can't undo at the moment without finer window depth/z control.
            //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);1
            if (ImGui::MenuItem("New", "Ctrl+N"))
            {
                //NewScene();
            }

            if (ImGui::MenuItem("Open...", "Ctrl+O"))
            {
                std::ifstream ifs("..\\File_1.txt");
                std::string content((std::istreambuf_iterator<char>(ifs)),
                    (std::istreambuf_iterator<char>()));

                GetApp()->GetEngine()->GetScene()->Deserialize(content);
                //OpenScene();
            }

            if (ImGui::MenuItem("Save", "Ctrl+S"))
            {
                //SaveScene();
                std::string json = GetApp()->GetEngine()->GetScene()->Serialize();
                std::ofstream file_handler;
                // File Open
                file_handler.open("..\\File_1.txt");

                // Write to the file
                file_handler << json;

                // File Close
                file_handler.close();
            }

            if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
            {
                //SaveSceneAs();
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
            ImGui::Separator();

            if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
            if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
            if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
            if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
            if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
            ImGui::Separator();

            if (ImGui::MenuItem("Close", NULL, false, &p_open != NULL))
                p_open = false;
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::End();

    gvm->draw_imgui();    
    hierarchy->draw_imgui(*GetApp()->GetEngine()->GetScene());
    properties->draw_imgui(GetApp()->GetEngine()->GetScene(), selected_go);
    SettingsWindow->draw_imgui();
}