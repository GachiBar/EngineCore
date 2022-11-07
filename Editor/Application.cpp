#include "Application.h"

#include <iostream>

#include "libs/Delegates.h"
#include "Layer.h"
#include "InputManager.h"
#include "Windows/WindowsWindow.h"
#include "../GameplaySystem/Component.h"
#include "InputCoreSystem/InputEvent/Events.h"
#include "InputCoreSystem/InputSettings.h"

const char* Application::kMonoLibPath = "vendor\\mono\\lib\\4.5";
const char* Application::kDllPath = "GameplayCore.dll";

Application::Application()
	: m_LayerStack(this)
	, m_JitDomain(kMonoLibPath, "KtripRuntime")
	, m_Domain("KtripDomain")
	, m_Assembly(m_Domain.get_assembly(kDllPath))
	, engine_(new engine::Engine(m_Domain, m_Assembly))
	, exit_code_(0)
{	
	mono::mono_domain::set_current_domain(m_Domain);
	engine::Scene::CacheMethods(m_Assembly);
	engine::GameObject::CacheMethods(m_Assembly);
	engine::Component::CacheMethods(m_Assembly);	
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
	EKeys::Initialize();

	//input_settings->AddActionMapping("Test", EKeys::A);
	//input_settings->AddActionMapping("Test", EKeys::W);
	//input_settings->AddActionMapping("Test", EKeys::SpaceBar);
	//input_settings->AddActionMapping("Test", EKeys::Enter);


	//input_settings->AddAxisMapping("TestAxis", FInputAxisKeyMapping{ EKeys::A,-1.f });
	//input_settings->AddAxisMapping("TestAxis", FInputAxisKeyMapping{ EKeys::D,1.f });
	//input_settings->AddAxisMapping("TestAxis", FInputAxisKeyMapping{ EKeys::W,0.5f });
	//input_settings->AddActionMapping("Test", EKeys::SpaceBar);

	InputManager::getInstance().input_settings->LoadKeyMappingsFromConfig();
	//input_settings->SaveKeyMappingsToFile();
	
	auto wnd = FWindowsWindow::Make();
	FGenericWindowDefinition wnd_def;
	wnd_def.Type = EWindowType::Normal;
	wnd_def.XDesiredPositionOnScreen = 640.f;
	wnd_def.YDesiredPositionOnScreen = 160.f;
	wnd_def.WidthDesiredOnScreen = 1286.f;
	wnd_def.HeightDesiredOnScreen = 726.f;
	wnd_def.TransparencySupport = EWindowTransparency::None;
	wnd_def.HasOSWindowBorder = true;
	wnd_def.AppearsInTaskbar = true;
	wnd_def.AcceptsInput = true;
	wnd_def.ActivationPolicy = EWindowActivationPolicy::Always;
	wnd_def.IsTopmostWindow = false;
	wnd_def.HasCloseButton = true;
	wnd_def.SupportsMinimize = true;
	wnd_def.IsModalWindow = false;
	wnd_def.IsRegularWindow = true;
	wnd_def.SizeWillChangeOften = false;
	wnd_def.ShouldPreserveAspectRatio = false;
	wnd_def.CornerRadius = 2;
	wnd_def.ExpectedMaxHeight = -1;
	wnd_def.ExpectedMaxWidth = -1;
	wnd_def.Title = L"HelloWindow";
	wnd_def.Opacity = 1.f;
	
	auto t = std::make_shared<FGenericWindowDefinition>(wnd_def);
	wnd->Initialize(this, t, GetModuleHandle(NULL), nullptr, true);

	auto wnd2 = FWindowsWindow::Make();
	wnd_def.AcceptsInput = false;
	wnd2->Initialize(this, t, GetModuleHandle(NULL), wnd, true);

	wnds.push_back(wnd);
	wnds.push_back(wnd2);

	wnd->Show();
	//wnd->SetWindowFocus();

	//wnd2->Show();
	wnd2->Enable(false);
	wnd2->Hide();
	SetFocus(wnd->GetHWnd());

	InputManager::getInstance().RegisterInputDevice(this);
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

	engine_->Initialize(static_cast<FWindowsWindow*>(wnds[0].get())->GetHWnd(), static_cast<FWindowsWindow*>(wnds[1].get())->GetHWnd(), wnds[0]->GetDefinition().WidthDesiredOnScreen, wnds[0]->GetDefinition().HeightDesiredOnScreen);

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

		for (const auto layer : m_LayerStack)
			layer->OnUpdate(engine_->kDt);;

		if(InputManager::getInstance().player_input->IsPressed(EKeys::Invalid))
			std::cout << "Pressed" << std::endl;

		if(InputManager::getInstance().player_input->WasActionJustPressed("Test2"))
		{
			std::cout << "Pressed" << std::endl;
		}
		if (InputManager::getInstance().player_input->IsActionPressed("Test2"))
		{
			std::cout << "StillPressed" << std::endl;
		}
		if (InputManager::getInstance().player_input->WasActionJustReleased("Test2"))
		{
			std::cout << "Released" << std::endl;
		}
		
		std::cout << InputManager::getInstance().player_input->GetKeyValue(EKeys::MouseX);

		InputManager::getInstance().Flush();

		engine_->BeginRender();

		for (const auto layer : m_LayerStack)
			layer->OnGuiRender();

		engine_->EndRender();
		
		for (const auto layer : m_LayerStack)
			layer->OnPostRender();
	}

	OnStop();

	engine_->Terminate();

	return exit_code_;
}

void Application::Close()
{
	is_exit_requested = true;
}

std::shared_ptr<FGenericWindow> Application::GetMainWindow()
{
	return wnds.at(0);
}

const mono::mono_assembly& Application::GetAssembly() const
{
	return m_Assembly;
}

engine::Engine* Application::GetEngine() const
{
	return engine_.get();
}

WNDPROC Application::GetWndProc()
{
	return Application::WndProc;
}

void Application::ApplyInput()
{
	std::shared_ptr<FInputEvent> IE;

	while (InputManager::getInstance().ReadEvent(IE))
	{
		IE->OnApplyInput(InputManager::getInstance().player_input->GetKeyStates());
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			/*
			if (CurrentEvent->Handled)
				break;
			(*it)->OnInputEvent(CurrentEvent.get());
			*/
		}
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
