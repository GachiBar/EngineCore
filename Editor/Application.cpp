#include "Application.h"

#include <iostream>

#include "Delegates.h"
#include "Layer.h"
#include "InputSystem/InputManager.h"
#include "ImGuiLayer.h"

Application::Application():m_LayerStack(this),engine_(new engine::Engine()), exit_code_(0)
{
}

void Application::PushLayer(Layer* layer)
{
	m_LayerStack.PushLayer(layer);
	layer->OnAttach();
}

void Application::PushOverlay(Layer* layer)
{
	m_LayerStack.PushOverlay(layer);
	layer->OnAttach();
}

void Application::OnSetup()
{

}

void Application::OnStart()
{
	auto io = ImGui::GetIO();

	//io->SetIniFilename("imgui.ini"); // We don't want to save .ini file
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	//for (const auto layer : m_LayerStack)
		//layer->OnGuiRender();

	//m_ImGuiLayer = new ImGuiLayer(&m_LayerStack);
	//PushOverlay(m_ImGuiLayer);
}

int Application::Run()
{
	OnSetup();
	if (exit_code_)
		return exit_code_;

	HINSTANCE instance = GetModuleHandle(nullptr);
	LPCWSTR window_name = L"Test window";
	LONG width = 800;
	LONG height = 600;

	RegisterWindowClass(GetModuleHandle(nullptr), window_name);
	HWND handle_old = CreateWindowInstance(instance, window_name, width, height);
	HWND handle_new = CreateWindowInstance(instance, window_name, width, height);

	ShowWindow(handle_old, SW_SHOW);
	ShowWindow(handle_new, SW_SHOW);

	engine_->Initialize(handle_old, handle_new, width, height);

	OnStart();
	if (exit_code_)
		return exit_code_;

	MSG msg = {};
	//bool is_exit_requested = false;

	while (!is_exit_requested) 
	{
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			
			if (msg.message == WM_QUIT) 
			{
				is_exit_requested = true;
			}
		}

		ApplyInput();
		engine_->RunFrame();

		InputManager::getInstance().Flush();

		engine_->GetRenderer().BeginFrame();

		engine_->GetRenderer().SetRenderData({});

		engine_->GetScene()->CallMethod("Render");

		//ImGui::ShowDemoWindow();
		
		for (const auto layer : m_LayerStack)
			layer->OnGuiRender();


		while (!engine_->GetRenderer().Present()) {
			engine_->GetRenderer().EndFrame();
		}

		engine_->GetRenderer().EndFrame();
	}

	OnStop();

	engine_->Terminate();

	return exit_code_;
}

void Application::Close()
{
	is_exit_requested = true;
}

engine::Engine* Application::GetEngine() const
{
	return engine_.get();
}

void Application::ApplyInput()
{
	std::shared_ptr<InputEvent> CurrentEvent= InputManager::getInstance().ReadEvent();
	while (CurrentEvent)
	{
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (CurrentEvent->Handled)
				break;
			(*it)->OnInputEvent(CurrentEvent.get());
		}
		CurrentEvent = CurrentEvent = InputManager::getInstance().ReadEvent();
	}
}

LRESULT CALLBACK Application::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	Application* application;

	if (msg == WM_CREATE) {
		const auto create_struct = reinterpret_cast<CREATESTRUCT*>(lparam);
		application = static_cast<Application*>(create_struct->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(application));
	}
	else {
		application = reinterpret_cast<Application*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}

	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
	{
		return 1;
	}
	if (!ImGui::GetCurrentContext())
		ImGui::CreateContext();

	if (msg == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0;
	}
	//else if ((io.WantCaptureMouse || io.WantCaptureKeyboard) && InputManager::IsInputMessage(msg))
	//{
		//return DefWindowProc(hwnd, msg, wparam, lparam);
	//}
	else if (InputManager::getInstance().IsInputMessage(msg) || msg == WM_KILLFOCUS) {
		InputManager::getInstance().ProcessInput(hwnd, msg, wparam, lparam);
	}		
	else if (application != nullptr && application->engine_->ProcessMessages(hwnd, msg, wparam, lparam)) {
		return true;
	}
	
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Application::RegisterWindowClass(HINSTANCE instance, LPCWSTR window_name) 
{
	WNDCLASSEX window_class;
	window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	window_class.lpfnWndProc = WndProc;
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0;
	window_class.hInstance = instance;
	window_class.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	window_class.hIconSm = window_class.hIcon;
	window_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
	window_class.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	window_class.lpszMenuName = nullptr;
	window_class.lpszClassName = window_name;
	window_class.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&window_class);
}

HWND Application::CreateWindowInstance(HINSTANCE instance, LPCWSTR window_name, LONG width, LONG height) {
	RECT window_rect = { 0, 0, width, height };
	AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, FALSE);

	return CreateWindowEx(
		WS_EX_APPWINDOW,
		window_name,
		window_name,
		WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME,
		(GetSystemMetrics(SM_CXSCREEN) - width) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - height) / 2,
		window_rect.right - window_rect.left,
		window_rect.bottom - window_rect.top,
		nullptr,
		nullptr,
		instance,
		this);
}
