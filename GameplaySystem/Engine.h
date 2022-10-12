#pragma once


#include <chrono>
#include <Windows.h>
#include "SimpleMath.h"

#include "RenderEngine.h"
#include "CSharpObject.h"

namespace engine {

using namespace std::chrono;

class Engine {
	const std::chrono::nanoseconds kTimestep =
		std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::milliseconds(16));

	const float	kDt = 16.0f / 1000;

public:
	CSharpObject* GetScene();
	void SetScene(CSharpObject* scene);
	RenderDevice& GetRenderer();

	void Initialize(HWND handle_old, HWND handle_new, UINT width, UINT height);
	void Terminate();
	void RunFrame();
	bool ProcessMessages(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	
private:
	static RenderDevice* current_device_; // For test;

	time_point<steady_clock> time_start_ = high_resolution_clock::now();
	nanoseconds lag_ = 0ns;
	nanoseconds ellapsed_ = 0ns;
		
	RenderDevice renderer_;
	CSharpObject* scene_;

	static void Internal_RegisterModel(size_t id);
	static void Internal_DrawModel(size_t id, DirectX::SimpleMath::Matrix model_matrix);

	void InitRenderer(HWND handle_old, HWND handle_new, size_t width, size_t height);
};

} // namespace engine

