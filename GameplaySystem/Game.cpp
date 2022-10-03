#include "pch.h"
#include "Game.h"

#include <fstream>
#include <iostream>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

namespace engine {

MonoDomain* s_RootDomain;
MonoDomain* s_AppDomain;

void InitMono()
{
	mono_set_assemblies_path("mono/lib");

	MonoDomain* rootDomain = mono_jit_init("MyScriptRuntime");
	if (rootDomain == nullptr)
	{
		// Maybe log some error here
		return;
	}

	// Store the root domain pointer
	s_RootDomain = rootDomain;

	// Create an App Domain
	s_AppDomain = mono_domain_create_appdomain(const_cast<char*>("MyAppDomain"), nullptr);
	mono_domain_set(s_AppDomain, true);
}

char* ReadBytes(const std::string& filepath, uint32_t* outSize)
{
	std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

	if (!stream)
	{
		// Failed to open the file
		return nullptr;
	}

	std::streampos end = stream.tellg();
	stream.seekg(0, std::ios::beg);
	uint32_t size = end - stream.tellg();

	if (size == 0)
	{
		// File is empty
		return nullptr;
	}

	char* buffer = new char[size];
	stream.read((char*)buffer, size);
	stream.close();

	*outSize = size;
	return buffer;
}

MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath)
{
	uint32_t fileSize = 0;
	char* fileData = ReadBytes(assemblyPath, &fileSize);

	// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
	MonoImageOpenStatus status;
	MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

	if (status != MONO_IMAGE_OK)
	{
		const char* errorMessage = mono_image_strerror(status);
		// Log some error message using the errorMessage data
		return nullptr;
	}

	MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
	mono_image_close(image);

	// Don't forget to free the file data
	delete[] fileData;

	return assembly;
}

void PrintAssemblyTypes(MonoAssembly* assembly)
{
	MonoImage* image = mono_assembly_get_image(assembly);

	// Get Metadata table of type definitions
	// All tables definitions - http://docs.go-mono.com/?link=xhtml%3adeploy%2fmono-api-metadata.html
	const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
	int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

	for (int32_t i = 0; i < numTypes; i++)
	{
		// Get current row with MONO_TYPEDEF_SIZE columns number
		uint32_t cols[MONO_TYPEDEF_SIZE];
		mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

		// Decode data
		const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
		const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

		printf("%s.%s\n", nameSpace, name);
	}
}

void Game::Run() {
	using namespace std::chrono;
	using clock = high_resolution_clock;

	InitMono();
	MonoAssembly* assembly = LoadCSharpAssembly("MonoPlayground.dll");
	PrintAssemblyTypes(assembly);

	Init();

	nanoseconds lag = 0ns;
	auto time_start = clock::now();
	bool is_exit_requested = false;

	// Game loop.
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

		// TODO: interpolation
		// TODO: update.
		// TODO: render.

		renderer_.BeginFrame();
		renderer_.SetRenderData({});

		while (!renderer_.Present()) {
			renderer_.EndFrame();
		}
		
		renderer_.EndFrame();		
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

void Game::Init() {
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
