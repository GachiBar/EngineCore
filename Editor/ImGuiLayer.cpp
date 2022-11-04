
#include "ImGuiLayer.h"


#include "Application.h"
#include "LayerStack.h"

#include "imgui/imgui_impl_dx11.h"

namespace Renderer
{
	class D3D11Renderer;
}

ImGuiLayer::ImGuiLayer(LayerStack* owner) : Layer(owner, "ImGuiLayer")
{
}

void ImGuiLayer::OnAttach()
{
	
	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;


	// OnSetup Dear ImGui style
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

void ImGuiLayer::OnDetach()
{
	ImGui::DestroyContext();
}

void ImGuiLayer::OnInputEvent(FInputEvent* e)
{
	if (m_BlockEvents)
	{
		ImGuiIO& io = ImGui::GetIO();
		//e->Handled |= e->IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
		//e->Handled |= e->IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
	}
}

void ImGuiLayer::Begin()
{
	ImGui_ImplDX11_NewFrame();
	ImGui::NewFrame();
	//ImGuizmo::BeginFrame();
}

void ImGuiLayer::End()
{
	ImGuiIO& io = ImGui::GetIO();
	auto app = owner_layer_stack->GetOwner();
	io.DisplaySize = ImVec2(800, 600);

	// Rendering
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		//ImGui::RenderPlatformWindowsDefault();
	}
}