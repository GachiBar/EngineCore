#pragma once


#include <chrono>
#include <Windows.h>

#include "RenderEngine.h"
#include "CSharpObject.h"


namespace engine {

	using namespace std::chrono;
class Game {
	const std::chrono::nanoseconds kTimestep =
		std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::milliseconds(16));

	const float	kDt = 16.0f / 1000;

public:
	CSharpObject* GetScene();
	void SetScene(CSharpObject* scene);

	void Initialize();
	void Terminate();

	void RunFrame();

	bool IsExiting() const;
private:
	static RenderDevice* current_device_; // For test;
	static bool flag_;

	time_point<steady_clock> time_start = high_resolution_clock::now();
	bool is_exit_requested = false;
	nanoseconds lag = 0ns;

	HWND handle_old_;
	HWND handle_new_;
	RenderDevice renderer_;
	CSharpObject* scene_;
	
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	static void RegisterModel(size_t id);
	static void DrawModel(size_t id);

	void RegisterWindowClass(HINSTANCE instance, LPCWSTR window_name);
	HWND CreateWindowInstance(HINSTANCE instance, LPCWSTR window_name, LONG width, LONG height);
	void InitRenderer(size_t width, size_t height);
};

} // namespace engine

