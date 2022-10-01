#pragma once

#include <chrono>
#include <Windows.h>

#include "RenderEngine.h"


namespace engine {

class Game {
	const std::chrono::nanoseconds kTimestep =
		std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::milliseconds(16));

	const float	kDt = 16.0f / 1000;

public:
	void Run();

// TODO: now I implement basic window creation here, but we should refactor this in future.
private:
	HWND handle_old_;
	HWND handle_new_;
	RenderDevice renderer_;
	
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	void Init();
	void RegisterWindowClass(HINSTANCE instance, LPCWSTR window_name);
	HWND CreateWindowInstance(HINSTANCE instance, LPCWSTR window_name, LONG width, LONG height);
	void InitRenderer(size_t width, size_t height);
};

} // namespace engine

