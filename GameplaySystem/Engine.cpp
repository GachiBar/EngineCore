#include "pch.h"
#include "Engine.h"
#include "CSharpDomain.h"
#include "CSharpAssembly.h"
#include "CSharpObject.h"

#include <fstream>
#include <iostream>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include "../Editor/InputSystem/InputManager.h"
#include <algorithm>

#include "string_view"
#include "MathematicsInternals.h"

namespace engine {

RenderDevice* Engine::current_device_ = nullptr;

CSharpObject* Engine::GetScene() {
	return scene_;
}

void Engine::SetScene(CSharpObject* scene) {
	scene_ = scene;
}

LRESULT Engine::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	Engine* engine = nullptr;

	if (msg == WM_CREATE) {
		const auto create_struct = reinterpret_cast<CREATESTRUCT*>(lparam);
		engine = static_cast<Engine*>(create_struct->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(engine));
	}
	else {
		engine = reinterpret_cast<Engine*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}

	if (msg == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0;
	}
	else if (InputManager::getInstance().IsInputMessage(msg))
		InputManager::getInstance().ProcessInput(hwnd, msg, wparam, lparam);
	else
	{
		if (engine->renderer_.ProcessMessages(hwnd, msg, wparam, lparam)) {
			return true;
		}
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Engine::Internal_RegisterModel(size_t id) {
	current_device_->RegisterModel(id, {
		{
			{{ -0.25, -0.25, 0 }, {},{}},
			{{  0.0 ,  0.25, 0 }, {},{}},
			{{  0.25, -0.25, 0 }, {},{}},			
		},
		{0,1,2},
		EPrimitiveType::PRIMITIVETYPE_TRIANGLELIST,
		1
	});
}

void Engine::Internal_DrawModel(size_t id, DirectX::SimpleMath::Matrix model_matrix) {
	current_device_->DrawModel(id, 0, model_matrix, ModelDefines::MRED);
}

void Engine::Initialize() {
	HINSTANCE instance = GetModuleHandle(nullptr);
	LPCWSTR window_name = L"Test window";
	LONG width = 800;
	LONG height = 600;

	RegisterWindowClass(GetModuleHandle(nullptr), window_name);
	handle_old_ = CreateWindowInstance(instance, window_name, width, height);
	handle_new_ = CreateWindowInstance(instance, window_name, width, height);

	ShowWindow(handle_old_, SW_SHOW);
	ShowWindow(handle_new_, SW_SHOW);
	InitRenderer(static_cast<size_t>(width), static_cast<size_t>(height));

	current_device_ = &renderer_;

	mono_add_internal_call("GameplayCore.EngineApi.Render::Internal_RegisterModel", Internal_RegisterModel);
	mono_add_internal_call("GameplayCore.EngineApi.Render::Internal_DrawModel", Internal_DrawModel);

	AddMathematicsInternalCalls();	

	scene_->CallMethod("Initialize");
}

void Engine::Terminate() {
	scene_->CallMethod("Terminate");
}

void Engine::RunFrame()
{
	using namespace std::chrono;
	using clock = high_resolution_clock;

	MSG msg = {};

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == WM_QUIT) {
			is_exit_requested_ = true;
		}
	}

	auto dt = clock::now() - time_start_;
	ellapsed_ += dt;
	time_start_ = clock::now();
	lag_ += duration_cast<nanoseconds>(dt);

	while (lag_ >= kTimestep) {
		lag_ -= kTimestep;
		scene_->CallMethod("FixedUpdate");
	}

	scene_->CallMethod("Update");

	renderer_.SetRenderData({
		duration<float>(ellapsed_).count(),
		matrix::Identity,
		matrix::Identity
	});

	renderer_.BeginFrame();
	
	renderer_.SetRenderData({});

	scene_->CallMethod("Render");

	while (!renderer_.Present()) {
		renderer_.EndFrame();
	}

	renderer_.EndFrame();
}

bool Engine::IsExiting() const
{
	return is_exit_requested_;
}

RenderDevice& Engine::GetRenderer()
{
	return renderer_;
}

void Engine::RegisterWindowClass(HINSTANCE instance, LPCWSTR window_name) {
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

HWND Engine::CreateWindowInstance(HINSTANCE instance, LPCWSTR window_name, LONG width, LONG height) {
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

void Engine::InitRenderer(size_t width, size_t height) {
	renderer_.InitDevice({
		handle_old_,
		handle_new_,
		width,
		height
	});

	renderer_.InitShaders("..\\..\\DX11RenderEngine\\DX11RenderEngine\\Shaders\\");
}

} // namespace engine