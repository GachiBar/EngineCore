#pragma once

#include "RenderEngine.h"
#include "../monowrapper/monopp/mono_domain.h"
#include "../monowrapper/monopp/mono_assembly.h"
#include "../monowrapper/monopp/mono_object.h"
#include "../monowrapper/monopp/mono_method_invoker.h"

#include <chrono>
#include <Windows.h>
#include <SimpleMath.h>

namespace engine {

using namespace std::chrono;

class Engine {
	friend class Application;

	const std::chrono::nanoseconds kTimestep =
		std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::milliseconds(16));

	const float	kDt = 16.0f / 1000;

public:
	Engine(
		const mono::mono_domain& domain,
		const mono::mono_assembly& assembly);

	mono::mono_object* GetScene();
	void SetScene(mono::mono_object* scene);
	void Initialize(HWND handle_old, HWND handle_new, UINT width, UINT height);
	void Terminate();

	void RunFrame();
	bool ProcessMessages(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	void InitRenderer(HWND handle_old, HWND handle_new, size_t width, size_t height);
	RenderDevice& GetRenderer();
private:
	RenderDevice renderer_;

	time_point<steady_clock> time_start_ = high_resolution_clock::now();
	nanoseconds lag_ = 0ns;
	nanoseconds ellapsed_ = 0ns;

	const::mono::mono_domain& domain_;
	const::mono::mono_assembly& assembly_;
		
	mono::mono_object* scene_;
	mono::mono_method_invoker* initialize_;
	mono::mono_method_invoker* terminate_;
	mono::mono_method_invoker* fixed_update_;
	mono::mono_method_invoker* update_;
	mono::mono_method_invoker* render_;

	static void Internal_RegisterModel(RenderDevice* renderer, size_t id);
	static void Internal_DrawModel(RenderDevice* renderer, size_t id, DirectX::SimpleMath::Matrix model_matrix);

	void SetupRendererInternalCalls();
	void InitializeSceneCalls();
	void TerminateSceneCalls();
};

} // namespace engine

