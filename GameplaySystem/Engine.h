#pragma once

#include "RenderEngine.h"
#include "../monowrapper/monopp/mono_assembly.h"

#include <chrono>
#include <Windows.h>
#include <SimpleMath.h>

#include "Scene.h"
#include "../Core/libs/loguru/loguru.hpp"

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

	static DirectX::SimpleMath::Matrix& GetViewMatrix();
	static DirectX::SimpleMath::Matrix& GetProjectionMatrix();
private:
	static DirectX::SimpleMath::Matrix m_projection;
	static DirectX::SimpleMath::Matrix m_view;

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

	mono::mono_property_invoker screen_width_property_;
	mono::mono_property_invoker screen_height_property_;

	mono::mono_property_invoker mouse_position_property_;

	std::shared_ptr<Scene> scene_;

	void InitRenderer(HWND handle_old, HWND handle_new, size_t width, size_t height);	

	void SetupRendererInternalCalls();
	void SetupInputInternalCalls();

	mono::mono_property GetProperty(std::string name_space, std::string clazz, std::string property);

	void SendTimeData();
	void SendScreenData();
	void SendInputData();

	static void Internal_RegisterModel(RenderDevice* renderer, size_t id);
	static void Internal_DrawModel(RenderDevice* renderer, size_t id, DirectX::SimpleMath::Matrix model_matrix);
	static void Internal_SetViewProjection(
		RenderDevice* renderer,
		float ellapsed,
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix projection);

	static bool Internal_IsPressed(MonoString* key_name);
	static bool Internal_WasJustPressed(MonoString* key_name);
	static bool Internal_WasJustReleased(MonoString* key_name);

	static bool Internal_IsActionPressed(MonoString* action_name);
	static bool Internal_WasActionJustPressed(MonoString* action_name);
	static bool Internal_WasActionJustReleased(MonoString* action_name);

	static float Internal_GetKeyValue(MonoString* key_name);
	static float Internal_GetAxisValue(MonoString* axis_name);


	static void Internal_RemoveLogMessage(MonoString* guid);
	static void Internal_Log(MonoString* message, bool bPrintToScreen, bool bPrintToLog, MonoString* guid);
	static void Internal_LogWarning(MonoString* message, bool bPrintToScreen, bool bPrintToLog);
	static void Internal_LogError(MonoString* message, bool bPrintToScreen, bool bPrintToLog);
	static void Internal_Log_Implementation(loguru::Verbosity verbosity, MonoString* message, bool bPrintToScreen, bool bPrintToLog);


};

} // namespace engine

