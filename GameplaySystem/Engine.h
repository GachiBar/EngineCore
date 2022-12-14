#pragma once

#include "RenderEngine.h"
#include "Runtime.h"
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

class Engine
{
public:
	static const float kDt;
	static const std::chrono::nanoseconds kTimestep;

	bool IsRunning();
	std::shared_ptr<Scene> GetScene();
	void SetScene(std::shared_ptr<Scene> scene);
	RenderDevice& GetRenderer();
	JPH::PhysicsSystem& GetPhysicsSystem();

	Engine(const Runtime& runtime);

	void Initialize(HWND handle,UINT width, UINT height);
	void Terminate();

	void Stop();
	void Start();

	void RunFrame();
	void BeginRender();
	void Render();
	void DebugRender();
	void EndRender();
	void Present();
	bool ProcessMessages(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:
	static const JPH::uint kMaxBodies = 65536;
	static const JPH::uint kNumBodyMutex = 0;
	static const JPH::uint kMaxBodyPairs = 65536;
	static const JPH::uint kMaxContactConstraints = 10240;

	static const int kCollisionSteps = 1;
	static const int kIntegrationSubSteps = 1;

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

	const Runtime& runtime_;

	Property renderer_property_;
	Property physics_system_property_;
	Property delta_time_property_;
	Property ellapsed_time_property_;
	Property screen_width_property_;
	Property screen_height_property_;
	Property mouse_position_property_;

	bool is_running_;

	void InitRenderer(HWND handle, size_t width, size_t height);	
	void InitPhysicsSystem();

	void SetupRendererInternalCalls();
	void SetupPhysicsInternalCalls();
	void SetupInputInternalCalls();
	void SetupLogInternalCalls();

	void SendTimeData();
	void SendScreenData();
	void SendInputData();

#pragma region Renderer

	static void Internal_RegisterModel(
		RenderDevice* renderer, 
		size_t id,
		MonoString* path);

	static void Internal_RegisterTexture(
		RenderDevice* renderer, 
		size_t id,
		MonoString* path);

	static void Internal_DrawModel(
		RenderDevice* renderer,
		size_t id,
		float metallic, 
		float roughness,
		DirectX::SimpleMath::Matrix model_matrix);

	static void Internal_DrawDirectionalLight(
		RenderDevice* renderer,
		DirectX::SimpleMath::Vector3 direction,
		float itencity,
		DirectX::SimpleMath::Color color);

	static void Internal_DrawCurve(
		RenderDevice* renderer, 
		MonoArray* points, 
		DirectX::SimpleMath::Vector3 color,
		DirectX::SimpleMath::Matrix model_matrix);

	static void Internal_SetViewProjection(
		RenderDevice* renderer, 
		float ellapsed, 
		DirectX::SimpleMath::Matrix view, 
		DirectX::SimpleMath::Matrix projection);

	static bool Internal_IsIdUsed(
		RenderDevice* renderer,
		size_t id);

	static bool Internal_IsMeshIdUsed(
			RenderDevice* renderer,
			size_t id);

	static bool Internal_IsTextureIdUsed(
			RenderDevice* renderer,
			size_t id);

#pragma endregion Renderer

#pragma region Physics

	static JPH::uint32 Internal_CreateBody(
		JPH::PhysicsSystem* physics_system,
		JPH::Vec3 position,
		JPH::Quat rotation,
		JPH::EMotionType motion_type);

	static void Internal_DestroyBody(
		JPH::PhysicsSystem* physics_system, 
		JPH::uint32 id);

	static void Internal_SetCollisionLayer(
		JPH::PhysicsSystem* physics_system,
		JPH::uint32 id,
		JPH::uint8 layer);

	static void Internal_SetEmptyShape(
		JPH::PhysicsSystem* physics_system,
		JPH::uint32 id);

	static void Internal_SetSphereShape(
		JPH::PhysicsSystem* physics_system, 
		JPH::uint32 id, 
		float radius);

	static void Internal_SetBoxShape(
		JPH::PhysicsSystem* physics_system, 
		JPH::uint32 id, 
		JPH::Vec3 half_extent);

	static void Internal_SetCapsuleShape(
		JPH::PhysicsSystem* physics_system,
		JPH::uint32 id,
		float half_height,
		float radius);

	static void Internal_SetMotionType(
		JPH::PhysicsSystem* physics_system, 
		JPH::uint32 id, 
		JPH::EMotionType motion_type);

	static void Internal_SetActive(
		JPH::PhysicsSystem* physics_system, 
		JPH::uint32 id, 
		bool is_active);

	static bool Internal_IsActive(
		JPH::PhysicsSystem* physics_system, 
		JPH::uint32 id);

	static JPH::Vec3 Internal_GetBodyPosition(
		JPH::PhysicsSystem* physics_system, 
		JPH::uint32 id);

	static void Internal_SetBodyPosition(
		JPH::PhysicsSystem* physics_system, 
		JPH::uint32 id, 
		JPH::Vec3 position);

	static JPH::Quat Internal_GetBodyRotation(
		JPH::PhysicsSystem* physics_system, 
		JPH::uint32 id);	

	static void Internal_SetBodyRotation(
		JPH::PhysicsSystem* physics_system, 
		JPH::uint32 id, 
		JPH::Quat rotation);

	static void Internal_AddForce(
		JPH::PhysicsSystem* physics_system, 
		JPH::uint32 id, 
		JPH::Vec3 force);

	static void Internal_AddImpulse(
		JPH::PhysicsSystem* physics_system,
		JPH::uint32 id,
		JPH::Vec3 impulse);

	static JPH::Vec3 Internal_GetLinearVelocity(
		JPH::PhysicsSystem* physics_system,
		JPH::uint32 id);

	static void Internal_SetLinearVelocity(
		JPH::PhysicsSystem* physics_system,
		JPH::uint32 id,
		JPH::Vec3 velocity);

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

	static void Internal_Log(
		MonoString* message, 
		bool should_print_to_screen, 
		bool should_print_to_log, 
		MonoString* guid);

	static void Internal_LogWarning(
		MonoString* message, 
		bool should_print_to_screen, 
		bool should_print_to_log, 
		MonoString* guid);

	static void Internal_LogError(
		MonoString* message, 
		bool should_print_to_screen, 
		bool should_print_to_log, 
		MonoString* guid);

	static void Internal_Log_Implementation(
		loguru::Verbosity verbosity, 
		MonoString* message, 
		bool should_print_to_screen, 
		bool should_print_to_log, 
		MonoString* guid);
};

} // namespace engine

