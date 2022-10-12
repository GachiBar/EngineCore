#include "pch.h"
#include "Game.h"
#include "CSharpDomain.h"
#include "CSharpAssembly.h"
#include "CSharpObject.h"

#include <fstream>
#include <iostream>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include "../Editor/InputSystem/InputManager.h"
#include <algorithm>


namespace engine {

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

	static auto GetKeyBoard = []()->Keyboard& { return InputManager::getInstance().GetKeyboardDevice(); };
	static auto GetMouse = []()->Mouse& {return InputManager::getInstance().GetMouseDevice(); };
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_KILLFOCUS:
		InputManager::getInstance().GetKeyboardDevice().ClearState();
		break;

		// ************ KEYBOARD MESSAGES ************ //
	case WM_KEYDOWN:
		if (!(lparam & 0x40000000) || InputManager::getInstance().GetKeyboardDevice().IsAutorepeatEnabled()) // no thank you on the autorepeat
		{
			GetKeyBoard().OnKeyPressed(static_cast<unsigned char>(wparam));
		}
		break;
	case WM_KEYUP:
		GetKeyBoard().OnKeyReleased(static_cast<unsigned char>(wparam));
		break;
	case WM_CHAR:
		GetKeyBoard().OnChar(static_cast<unsigned char>(wparam));
		break;
		// ************ END KEYBOARD MESSAGES ************ //

		// ************ MOUSE MESSAGES ************ //
	case WM_MOUSEMOVE:
	{
		POINTS pt = MAKEPOINTS(lparam);
		//TODO remove magic numbers
		if (pt.x > 0 && pt.x < 800 && pt.y > 0 && pt.y < 600)
		{
			GetMouse().OnMouseMove(pt.x, pt.y);
			if (!GetMouse().IsInWindow())
			{
				SetCapture(hwnd);
				GetMouse().OnMouseEnter();
			}
		}
		else
		{
			if (wparam & (MK_LBUTTON | MK_RBUTTON))
			{
				pt.x = std::max(short(0), pt.x);
				//TODO remove magic numbers
				pt.x = std::min(short(800 - 1), pt.x);
				pt.y = std::max(short(0), pt.y);
				//TODO remove magic numbers
				pt.y = std::min(short(600 - 1), pt.y);
				GetMouse().OnMouseMove(pt.x, pt.y);
			}
			else
			{
				ReleaseCapture();
				GetMouse().OnMouseLeave();
				GetMouse().OnLeftReleased(pt.x, pt.y);
				GetMouse().OnRightReleased(pt.x, pt.y);
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lparam);
		GetMouse().OnLeftPressed(pt.x, pt.y);
		SetForegroundWindow(hwnd);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lparam);
		GetMouse().OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lparam);
		GetMouse().OnLeftReleased(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lparam);
		GetMouse().OnRightReleased(pt.x, pt.y);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const MOUSEHOOKSTRUCTEX* mhs = (const MOUSEHOOKSTRUCTEX*)lparam;
		short delta = HIWORD(mhs->mouseData);

		const POINTS pt = MAKEPOINTS(lparam);
		if (delta > 0)
		{
			GetMouse().OnWheelUp(pt.x, pt.y,delta);
		}
		else if (delta < 0)
		{
			GetMouse().OnWheelDown(pt.x, pt.y,delta);
		}
		break;
	}
	// ************ END MOUSE MESSAGES ************ //
	default:
		if (game->renderer_.ProcessMessages(hwnd, msg, wparam, lparam)) {
			return true;
		}

		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
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
}

void Game::InitializeScene() const
{
	scene_->CallMethod("Initialize");
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

	renderer_.BeginFrame();
	renderer_.SetRenderData({});

	while (!renderer_.Present()) {
		renderer_.EndFrame();
	}

	renderer_.EndFrame();
}

bool Game::IsExiting() const
{
	return is_exit_requested;
}

RenderDevice& Game::GetRenderer()
{
	return renderer_;
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

	renderer_.InitShaders(".\\KtripEngine\\DX11RenderEngine\\DX11RenderEngine\\Shaders");
}

} // namespace engine
