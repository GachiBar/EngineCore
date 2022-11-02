#pragma once

#include "RenderEngine.h"
#include "../monowrapper/monopp/mono_assembly.h"

#include <chrono>
#include <Windows.h>
#include <SimpleMath.h>

#include "Scene.h"

namespace engine {

using namespace std::chrono;

class Engine {
public:
	const std::chrono::nanoseconds kTimestep =
		std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::milliseconds(16));

	const float	kDt = 16.0f / 1000;

	std::shared_ptr<Scene> GetScene();
	void SetScene(std::shared_ptr<Scene> scene);
	RenderDevice& GetRenderer();

	Engine(
		const mono::mono_domain& domain,
		const mono::mono_assembly& assembly);

	void Initialize(HWND handle_old, HWND handle_new, UINT width, UINT height);
	void Terminate();

	void RunFrame();
	void BeginRender();
	void EndRender();
	bool ProcessMessages(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);	
	
private:
	RenderDevice renderer_;

	time_point<steady_clock> time_start_ = high_resolution_clock::now();
	nanoseconds dt_ = 0ns;
	nanoseconds lag_ = 0ns;
	nanoseconds ellapsed_ = 0ns;

	const::mono::mono_domain& domain_;
	const::mono::mono_assembly& assembly_;

	mono::mono_property_invoker renderer_property_;
	mono::mono_property_invoker delta_time_property_;
	mono::mono_property_invoker ellapsed_time_property_;

	std::shared_ptr<Scene> scene_;

	static void Internal_RegisterModel(RenderDevice* renderer, size_t id);
	static void Internal_DrawModel(RenderDevice* renderer, size_t id, DirectX::SimpleMath::Matrix model_matrix);
	static void Internal_SetViewProjection(
		RenderDevice* renderer, 
		float ellapsed, 
		DirectX::SimpleMath::Matrix view, 
		DirectX::SimpleMath::Matrix projection);

	void InitRenderer(HWND handle_old, HWND handle_new, size_t width, size_t height);
	mono::mono_property GetProperty(std::string name_space, std::string clazz, std::string property);
	void SetupRendererInternalCalls();
	void SendTimeData();
};

} // namespace engine

