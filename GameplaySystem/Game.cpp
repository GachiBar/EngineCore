#include "pch.h"
#include "Game.h"

namespace engine {

void Game::Run() {
	using namespace std::chrono;
	using clock = high_resolution_clock;

	Init();

	nanoseconds lag = 0ns;
	auto time_start = clock::now();
	bool is_exit_requested = false;

	while (!is_exit_requested) {
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

			// TODO: fixed update.
		}
		
		static float intensity = 1.0f;
		static int radius = 1.0f;
		static float sigma = 1.0f;
		static float threshold = 1.0f;
		static int kernel = 1.0f;

		renderer_.BeginFrame();

		ImGui::Begin("BloomM settings.");

		ImGui::SliderFloat("Intensity.", &intensity, 0.0f, 10.0f);

		ImGui::SliderInt("Radius.", &radius, 0, 10);

		ImGui::SliderFloat("Sigma.", &sigma, 0.01f, 5.0f);

		ImGui::SliderFloat("Threshold.", &threshold, 0.0f, 1.0);

		ImGui::SliderInt("Kernel.", &kernel, 1, 10);

		ImGui::End();

		renderer_.SetRenderData({});

		while (!renderer_.Present()) {
			renderer_.EndFrame();
		}
		
		renderer_.EndFrame();		

		// TODO: interpolation
		// TODO: update.
		// TODO: render.
	}
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

	if (game->renderer_.ProcessMessages(hwnd, msg, wparam, lparam)) {
		return true;
	}

	//return game->renderer_.ProcessMessages(hwnd, msg, wparam, lparam);
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Game::Init() {
	HINSTANCE instance = GetModuleHandle(nullptr);
	LPCWSTR window_name = L"Test window";
	LONG width = 800;
	LONG height = 600;

	RegisterWindowClass(GetModuleHandle(nullptr), window_name);
	handle_1_ = CreateWindowInstance(instance, window_name, width, height);
	handle_2_ = CreateWindowInstance(instance, window_name, width, height);

	ShowWindow(handle_1_, SW_SHOW);
	ShowWindow(handle_2_, SW_SHOW);
	//SetForegroundWindow(handle_);
	//SetFocus(handle_);

	renderer_.InitDevice({
		handle_1_,
		handle_2_,
		(size_t) width,
		(size_t) height
	});

	renderer_.InitShaders(".\\KtripEngine\\DX11RenderEngine\\DX11RenderEngine\\Shaders");
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

} // namespace engine
