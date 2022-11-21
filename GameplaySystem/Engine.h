#pragma once

#include "RenderEngine.h"
#include "Scene.h"
#include "BPLayerInterfaceImplementation.h"
#include "../monowrapper/monopp/mono_assembly.h"

#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Physics/PhysicsSystem.h>

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
};

} // namespace engine

