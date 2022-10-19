#include "EditorLayer.h"
#include "Application.h"
#include "LayerStack.h"


#include "InputSystem/InputEvent.h"
#include "ImGuizmo/ImGuizmo.h"
#include "libs/nameof.hpp"
#include "imgui/imgui_sugar.hpp"


namespace Renderer
{
	class D3D11Renderer;
}

EditorLayer::EditorLayer(LayerStack* owner) : Layer(owner, "EditorLayer"), selected_go(nullptr)
{
}

void EditorLayer::OnAttach()
{
	gvm = std::make_shared<GameViewWindow>();
	hierarchy = std::make_shared<SceneHierarchyWindow>();
	properties = std::make_shared<PropertyWindow>();

	hierarchy.get()->OnSelectGameObjectInHierarchy.BindLambda([&](std::shared_ptr<mono::mono_object>& go)
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
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove;
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
	with_Window("DockSpace Demo", &p_open, window_flags)
	{
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

		with_MenuBar
		{
			with_Menu("File")
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
				{
					//NewScene();
				}

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
				{
					//OpenScene();
				}

				if (ImGui::MenuItem("Save", "Ctrl+S"))
				{
					//SaveScene();
				}

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
				{
					//SaveSceneAs();
				}

				if (ImGui::MenuItem("Exit"))
				{
					GetApp()->Close();
				}
			}
		}

		with_MenuBar
		{
			with_Menu("Options")
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
				with_MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)
				{
					dockspace_flags ^= ImGuiDockNodeFlags_NoSplit;
				}
				with_MenuItem("Flag: NoResize", "", (dockspace_flags& ImGuiDockNodeFlags_NoResize) != 0)
				{
					dockspace_flags ^= ImGuiDockNodeFlags_NoResize;
				}
				with_MenuItem("Flag: NoDockingInCentralNode", "",
				              (dockspace_flags& ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)
				{
					dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
				}
				with_MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags& ImGuiDockNodeFlags_AutoHideTabBar) != 0)
				{
					dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
				}
				with_MenuItem("Flag: PassthruCentralNode", "",
				              (dockspace_flags& ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)
				{
					dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
				}
			}
		}
	}

	gvm->draw_imgui();
	hierarchy->draw_imgui(*GetApp()->GetEngine()->GetScene());
	properties->draw_imgui(selected_go);
}

void EditorLayer::SetBlockEvents(bool block)
{
	m_BlockEvents = block;
}
