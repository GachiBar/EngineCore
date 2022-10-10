#include "pch.h"
#include "Game.h"
#include "CSharpDomain.h"
#include "CSharpAssembly.h"
#include "CSharpObject.h"

#include <fstream>
#include <iostream>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

namespace engine {

RenderDevice* Game::current_device_ = nullptr;
bool Game::flag_ = false;

CSharpObject* Game::GetScene() {
	return scene_;
}

void Game::SetScene(CSharpObject* scene) {
	scene_ = scene;
}

LRESULT Game::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	Game* game = nullptr;

	if (msg == WM_CREATE) {
		const auto create_struct = reinterpret_cast<CREATESTRUCT*>(lparam);
		game = static_cast<Game*>(create_struct->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(game));
	}
	else {
		game = reinterpret_cast<Game*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}

	switch (msg)
	{
	case WM_DESTROY: {
		PostQuitMessage(0);
		return 0;
	}
	default:
		if (game->renderer_.ProcessMessages(hwnd, msg, wparam, lparam)) {
			return true;
		}

		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
}

void Game::RegisterModel(size_t id) {
	flag_ = true;
	current_device_->RegisterModel(id, {
		{
			{{ 0.25,0.5,0}, {},{}},
			{{ 0.5,0.5,0}, {},{}},
			{{ 0.5,0.25,0}, {},{}},
		},
		{0,1,2,1,0,2},
		EPrimitiveType::PRIMITIVETYPE_TRIANGLELIST,
		2
	});
}

void Game::DrawModel(size_t id) {
	flag_ = true;
	current_device_->DrawModel(id, 0, {}, ModelDefines::MRED);
}

void Game::Initialize() {
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
	mono_add_internal_call("GameplayCore.Renderer::RegisterModel", RegisterModel);
	mono_add_internal_call("GameplayCore.Renderer::DrawModel", DrawModel);

	scene_->CallMethod("Initialize");
}

void Game::Terminate() {
	scene_->CallMethod("Terminate");
}

void Game::RunFrame()
{
	using namespace std::chrono;
	using clock = high_resolution_clock;

	MSG msg = {};

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == WM_QUIT) {
			is_exit_requested = true;
		}
	}

	auto dt = clock::now() - time_start;
	time_start = clock::now();
	lag += duration_cast<nanoseconds>(dt);

	while (lag >= kTimestep) {
		lag -= kTimestep;
		scene_->CallMethod("FixedUpdate");
	}

	scene_->CallMethod("Update");

	//renderer_.SetRenderData({
	//	std::chrono::duration<float>(dt).count(),
	//	matrix::Identity,
	//	matrix::Identity
	//});

	renderer_.BeginFrame();
	
	renderer_.SetRenderData({});
	
	bool t = Game::flag_;
	scene_->CallMethod("Render");

	while (!renderer_.Present()) {
		renderer_.EndFrame();
	}

	renderer_.EndFrame();
}

bool Game::IsExiting() const
{
	return is_exit_requested;
}

void Game::RegisterWindowClass(HINSTANCE instance, LPCWSTR window_name) {
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

HWND Game::CreateWindowInstance(HINSTANCE instance, LPCWSTR window_name, LONG width, LONG height) {
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

void Game::InitRenderer(size_t width, size_t height) {
	renderer_.InitDevice({
		handle_old_,
		handle_new_,
		width,
		height
	});

	renderer_.InitShaders("..\\..\\..\\KtripEngine\\DX11RenderEngine\\DX11RenderEngine\\Shaders\\");
}

} // namespace engine
