#pragma once

#include "RenderEngine.h"
#include "Scene.h"
#include "BPLayerInterfaceImplementation.h"
#include "../monowrapper/monopp/mono_assembly.h"
#include "../Core/libs/loguru/loguru.hpp"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Core/JobSystemThreadPool.h>

#include <chrono>
#include <Windows.h>
#include <SimpleMath.h>

JPH_SUPPRESS_WARNINGS

namespace engine {

using namespace std::chrono;

class Engine {
public:
	static const float kDt;
	static const std::chrono::nanoseconds kTimestep;

	std::shared_ptr<Scene> GetScene();
	void SetScene(std::shared_ptr<Scene> scene);
	RenderDevice& GetRenderer();
	JPH::PhysicsSystem& GetPhysicsSystem();

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
	static const JPH::uint kMaxBodies = 65536;
	static const JPH::uint kNumBodyMutex = 0;
	static const JPH::uint kMaxBodyPairs = 65536;
	static const JPH::uint kMaxContactConstraints = 10240;

	RenderDevice renderer_;

	JPH::TempAllocatorImpl temp_allocator_;
	JPH::JobSystemThreadPool job_system_;
	BPLayerInterfaceImplementation layer_interface_;
	JPH::PhysicsSystem physics_system_;

	std::shared_ptr<Scene> scene_;

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

	void InitRenderer(HWND handle_old, HWND handle_new, size_t width, size_t height);	
	void InitPhysicsSystem();

	void SetupRendererInternalCalls();
	void SetupInputInternalCalls();

	mono::mono_property GetProperty(std::string name_space, std::string clazz, std::string property);

	void SendTimeData();
	void SendScreenData();
	void SendInputData();

#pragma region Renderer

	static void Internal_RegisterModel(RenderDevice* renderer, size_t id);
	static void Internal_DrawModel(RenderDevice* renderer, size_t id, DirectX::SimpleMath::Matrix model_matrix);
	static void Internal_SetViewProjection(
		RenderDevice* renderer, 
		float ellapsed, 
		DirectX::SimpleMath::Matrix view, 
		DirectX::SimpleMath::Matrix projection);

#pragma endregion Renderer

#pragma region Physics

	static JPH::uint32 Internal_CreateSphereBody(
		JPH::PhysicsSystem* physics_system,
		float radius, 
		JPH::Vec3 position, 
		JPH::Quat rotation, 
		JPH::EMotionType motion_type, 
		JPH::uint8 layer);
	static JPH::uint32 Internal_CreateBoxBody(
		JPH::PhysicsSystem* physics_system,
		JPH::Vec3 half_extend, 
		JPH::Vec3 position, 
		JPH::Quat rotation, 
		JPH::EMotionType motion_type, 
		JPH::uint8 layer);
	static void Internal_DestroyBody(JPH::PhysicsSystem* physics_system, JPH::uint32 id);
	static void Internal_SetActive(JPH::PhysicsSystem* physics_system, JPH::uint32 id, bool is_active);
	static void Internal_GetBodyPositionAndRotation(
		JPH::PhysicsSystem* physics_system, 
		JPH::uint32 id, 
		JPH::Vec3& position, 
		JPH::Quat& rotation);

#pragma endregion Physics

#pragma region Inputs

	static bool Internal_IsPressed(MonoString* key_name);
	static bool Internal_WasJustPressed(MonoString* key_name);
	static bool Internal_WasJustReleased(MonoString* key_name);

	static bool Internal_IsActionPressed(MonoString* action_name);
	static bool Internal_WasActionJustPressed(MonoString* action_name);
	static bool Internal_WasActionJustReleased(MonoString* action_name);

	static float Internal_GetKeyValue(MonoString* key_name);
	static float Internal_GetAxisValue(MonoString* axis_name);

#pragma endregion Inputs

	static void Internal_RemoveLogMessage(MonoString* guid);
	static void Internal_Log(MonoString* message, bool bPrintToScreen, bool bPrintToLog, MonoString* guid);
	static void Internal_LogWarning(MonoString* message, bool bPrintToScreen, bool bPrintToLog, MonoString* guid);
	static void Internal_LogError(MonoString* message, bool bPrintToScreen, bool bPrintToLog, MonoString* guid);
	static void Internal_Log_Implementation(loguru::Verbosity verbosity, MonoString* message, bool bPrintToScreen, bool bPrintToLog, MonoString* guid);
};

} // namespace engine

