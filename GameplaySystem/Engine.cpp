#include "pch.h"
#include "Engine.h"
#include "MathematicsInternals.h"
#include "../monowrapper/monopp/mono_method.h"
#include "../monowrapper/monopp/mono_method_invoker.h"
#include "../monowrapper/monopp/mono_property_invoker.h"
#include "../Editor/InputSystem/InputManager.h"

#include <fstream>
#include <iostream>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <algorithm>

namespace engine {

Engine::Engine(const mono::mono_domain& domain, const mono::mono_assembly& assembly)
	: handle_old_(0)
	, handle_new_(0)
	, domain_(domain)
	, assembly_(assembly)
	, scene_(nullptr)
	, initialize_(nullptr)
	, terminate_(nullptr)
	, fixed_update_(nullptr)
	, update_(nullptr)
	, render_(nullptr)
{}

mono::mono_object* Engine::GetScene() {
	return scene_;
}

void Engine::SetScene(mono::mono_object* scene) {
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

void Engine::Internal_RegisterModel(RenderDevice* renderer, size_t id) {
	renderer->RegisterModel(id, {
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

void Engine::Internal_DrawModel(RenderDevice* renderer, size_t id, DirectX::SimpleMath::Matrix model_matrix) {
	renderer->DrawModel(id, 0, model_matrix, ModelDefines::MRED);
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

	SetupRendererInternalCalls();
	SetupMathematicsInternalCalls();	
	InitializeSceneCalls();

	initialize_->invoke(*scene_);
}

void Engine::Terminate() {
	terminate_->invoke(*scene_);

	TerminateSceneCalls();
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

	SendDeltaTime(duration<float>(dt).count());

	while (lag_ >= kTimestep) {
		lag_ -= kTimestep;
		fixed_update_->invoke(*scene_);
	}

	update_->invoke(*scene_);

	renderer_.BeginFrame();
	
	renderer_.SetRenderData({
		duration<float>(ellapsed_).count(),
		matrix::Identity,
		matrix::Identity
	});

	render_->invoke(*scene_);

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

void Engine::SetupRendererInternalCalls() {
	mono_add_internal_call("GameplayCore.EngineApi.Render::Internal_RegisterModel", Internal_RegisterModel);
	mono_add_internal_call("GameplayCore.EngineApi.Render::Internal_DrawModel", Internal_DrawModel);

	auto renderer_type = assembly_.get_type("GameplayCore.EngineApi", "Render");
	auto renderer_property = renderer_type.get_property("Renderer");
	mono::mono_property_invoker renderer_property_invoker(renderer_property);
	renderer_property_invoker.set_value(&renderer_);
}

void Engine::InitializeSceneCalls() {
	mono::mono_method initialize_method(scene_->get_type(), "Initialize", 0);
	initialize_ = new mono::mono_method_invoker(initialize_method);

	mono::mono_method terminate_method(scene_->get_type(), "Terminate", 0);
	terminate_ = new mono::mono_method_invoker(terminate_method);

	mono::mono_method fixed_update_method(scene_->get_type(), "FixedUpdate", 0);
	fixed_update_ = new mono::mono_method_invoker(fixed_update_method);

	mono::mono_method update_method(scene_->get_type(), "Update", 0);
	update_ = new mono::mono_method_invoker(update_method);

	mono::mono_method render_method(scene_->get_type(), "Render", 0);
	render_ = new mono::mono_method_invoker(render_method);
}

void Engine::TerminateSceneCalls() {
	delete initialize_;
	delete terminate_;
	delete fixed_update_;
	delete update_;
	delete render_;
}

void Engine::SendDeltaTime(float dt) {
	auto time_type = assembly_.get_type("GameplayCore", "Time");
	auto delta_time_property = time_type.get_property("DeltaTime");
	mono::mono_property_invoker delta_time_property_invoker(delta_time_property);
	delta_time_property_invoker.set_value(&dt);
}

} // namespace engine
