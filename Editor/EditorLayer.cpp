#include "EditorLayer.h"

//#include <imgui/imgui.h>

#include "Application.h"
#include "LayerStack.h"
//#include "../../FNADx11RenderDevice/include/D3D11Renderer.h"
//#include "../../DX11RenderEngine/DX11RenderEngine/source/CoreRenderSystem/Graphics.h"
#include <iostream>

#include "InputSystem/InputEvent.h"
#include "ImGuizmo/ImGuizmo.h"
#include "libs/nameof.hpp"
//#include "imgui/backends/imgui_impl_win32.h"
//#include "imgui/backends/imgui_impl_dx11.h"

namespace Renderer
{
	class D3D11Renderer;
}

EditorLayer::EditorLayer(LayerStack* owner) : Layer(owner,"EditorLayer")
{
}

void EditorLayer::OnAttach()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;


	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	
	//ImGui_ImplWin32_Init(owner_layer_stack->GetOwner()->GetEngine()->handle_new_);
	//ImGui_ImplDX11_Init(dynamic_cast<Renderer::D3D11Renderer*>(owner_layer_stack->GetOwner()->GetEngine()->GetRenderer().gfx->pRenderer)->device.Get(), dynamic_cast<Renderer::D3D11Renderer*>(owner_layer_stack->GetOwner()->GetEngine()->GetRenderer().gfx->pRenderer)->context.Get());
}

void EditorLayer::OnDetach()
{
	ImGui::DestroyContext();
}

void EditorLayer::OnInputEvent(InputEvent* e)
{
	auto t = NAMEOF_TYPE_RTTI(*e);
	std::cout << t << std::endl;
	if (m_BlockEvents)
	{
		ImGuiIO& io = ImGui::GetIO();
		e->Handled |= e->IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
		e->Handled |= e->IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
	}
}

void EditorLayer::Begin()
{
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}

void EditorLayer::End()
{
	ImGuiIO& io = ImGui::GetIO();
	auto app = owner_layer_stack->GetOwner();
	io.DisplaySize = ImVec2(800,600);

	// Rendering
	ImGui::Render();
}
