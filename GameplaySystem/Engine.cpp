#include "pch.h"
#include "Engine.h"
#include "ModelLoader.h"
#include "GameObject.h"
#include "Scene.h"
#include "TextureLoader.h"
#include "../InputSystem/InputManager.h"
#include "../Logger/LogManager.h"
#include "../monowrapper/monopp/mono_method.h"
#include "../monowrapper/monopp/mono_method_invoker.h"
#include "../monowrapper/monopp/mono_property_invoker.h"
#include "../monowrapper/monopp/mono_array.h"

#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

#include <fstream>
#include <iostream>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <algorithm>

JPH_SUPPRESS_WARNINGS

namespace engine {

const float Engine::kDt = 16.0f / 1000;
const std::chrono::nanoseconds Engine::kTimestep = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::milliseconds(16));

std::shared_ptr<Scene> Engine::GetScene() {
	return scene_;
}

void Engine::SetScene(std::shared_ptr<Scene> scene) {
	scene_ = scene;
}

RenderDevice& Engine::GetRenderer() {
	return renderer_;
}

JPH::PhysicsSystem& Engine::GetPhysicsSystem() {
	return physics_system_;
}

Engine::Engine(const Runtime& runtime)
	: temp_allocator_(10 * 1024 * 1024)
	, job_system_(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1)
	, scene_(nullptr)
	, runtime_(runtime)
	, renderer_property_(runtime_.GetType(Types::kRenderApi).GetProperty("Renderer"))
	, physics_system_property_(runtime_.GetType(Types::kPhysicsApi).GetProperty("PhysicsSystem"))
	, delta_time_property_(runtime_.GetType(Types::kTime).GetProperty("DeltaTime"))
	, ellapsed_time_property_(runtime_.GetType(Types::kTime).GetProperty("EllapsedTime"))
	, screen_width_property_(runtime_.GetType(Types::kScreen).GetProperty("Width"))
	, screen_height_property_(runtime_.GetType(Types::kScreen).GetProperty("Height"))
	, mouse_position_property_(runtime_.GetType(Types::kInput).GetProperty("MousePosition"))
{}

void Engine::Initialize(HWND handle, UINT width, UINT height) {
	InitRenderer(handle, static_cast<size_t>(width), static_cast<size_t>(height));
	InitPhysicsSystem();
	SetupRendererInternalCalls();
	SetupPhysicsInternalCalls();
	SetupInputInternalCalls();
}

void Engine::Terminate() {
	scene_->Terminate();

	delete JPH::Factory::sInstance;
	JPH::Factory::sInstance = nullptr;
}

void Engine::RunFrame() {
	using namespace std::chrono;
	using clock = high_resolution_clock;

	dt_ = clock::now() - time_start_;
	ellapsed_ += dt_;
	time_start_ = clock::now();
	lag_ += duration_cast<nanoseconds>(dt_);

	SendTimeData();
	SendScreenData();
	SendInputData();

	while (lag_ >= kTimestep) 	{
		lag_ -= kTimestep;
		scene_->FixedUpdate();
		physics_system_.Update(kDt, kCollisionSteps, kIntegrationSubSteps, &temp_allocator_, &job_system_);
	}

	scene_->Update();
}

void Engine::BeginRender() {
	renderer_.BeginFrame();
	scene_->Render();

	while (!renderer_.Present()) {
		renderer_.EndFrame();
		renderer_.ReloadShaders();
		renderer_.BeginFrame();
	};
}

void Engine::EndRender() {
	renderer_.EndFrame();
}

bool Engine::ProcessMessages(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	return renderer_.ProcessMessages(hwnd, msg, wparam, lparam);
}

void Engine::InitPhysicsSystem()
{
	JPH::Factory::sInstance = new JPH::Factory();

	JPH::RegisterTypes();

	physics_system_.Init(
		kMaxBodies,
		kNumBodyMutex,
		kMaxBodyPairs,
		kMaxContactConstraints,
		layer_interface_,
		BroadPhaseLayers::IsCanCollide,
		CollisionLayers::IsCanCollide);
}

void Engine::InitRenderer(HWND handle, size_t width, size_t height) {
	renderer_.CreateDevice(
	{
		handle,
		{
			width,
			height,
			width,
			height
		}
	});
	renderer_.InitDevice({ "..\\DX11RenderEngine\\GachiRenderSystem\\Shaders\\" });
}

void Engine::SetupRendererInternalCalls() {
	mono_add_internal_call("GameplayCore.EngineApi.RenderApi::Internal_RegisterModel", Internal_RegisterModel);
	mono_add_internal_call("GameplayCore.EngineApi.RenderApi::Internal_DrawModel", Internal_DrawModel);
	mono_add_internal_call("GameplayCore.EngineApi.RenderApi::Internal_DrawCurve", Internal_DrawCurve);
	mono_add_internal_call("GameplayCore.EngineApi.RenderApi::Internal_SetViewProjection", Internal_SetViewProjection);

	renderer_property_.SetValue(&renderer_);
}

void Engine::SetupPhysicsInternalCalls() 
{
	mono_add_internal_call("GameplayCore.EngineApi.PhysicsApi::Internal_CreateSphereBody", Internal_CreateSphereBody);
	mono_add_internal_call("GameplayCore.EngineApi.PhysicsApi::Internal_CreateBoxBody", Internal_CreateBoxBody);
	mono_add_internal_call("GameplayCore.EngineApi.PhysicsApi::Internal_DestroyBody", Internal_DestroyBody);
	mono_add_internal_call("GameplayCore.EngineApi.PhysicsApi::Internal_SetSphereShape", Internal_SetSphereShape);
	mono_add_internal_call("GameplayCore.EngineApi.PhysicsApi::Internal_SetBoxShape", Internal_SetBoxShape);	
	mono_add_internal_call("GameplayCore.EngineApi.PhysicsApi::Internal_SetMotionType", Internal_SetMotionType);
	mono_add_internal_call("GameplayCore.EngineApi.PhysicsApi::Internal_SetActive", Internal_SetActive);
	mono_add_internal_call("GameplayCore.EngineApi.PhysicsApi::Internal_IsActive", Internal_IsActive);
	mono_add_internal_call("GameplayCore.EngineApi.PhysicsApi::Internal_GetBodyPosition", Internal_GetBodyPosition);
	mono_add_internal_call("GameplayCore.EngineApi.PhysicsApi::Internal_SetBodyPosition", Internal_SetBodyPosition);
	mono_add_internal_call("GameplayCore.EngineApi.PhysicsApi::Internal_GetBodyRotation", Internal_GetBodyRotation);
	mono_add_internal_call("GameplayCore.EngineApi.PhysicsApi::Internal_SetBodyRotation", Internal_SetBodyRotation);
	mono_add_internal_call("GameplayCore.EngineApi.PhysicsApi::Internal_AddForce", Internal_AddForce);

	physics_system_property_.SetValue(&physics_system_);
}

void Engine::SetupInputInternalCalls() {
	mono_add_internal_call("GameplayCore.Input::Internal_IsPressed", Internal_IsPressed);
	mono_add_internal_call("GameplayCore.Input::Internal_WasJustPressed", Internal_WasJustPressed);
	mono_add_internal_call("GameplayCore.Input::Internal_Internal_WasJustReleased", Internal_WasJustReleased);

	mono_add_internal_call("GameplayCore.Input::Internal_IsActionPressed", Internal_IsActionPressed);
	mono_add_internal_call("GameplayCore.Input::Internal_WasActionJustPressed", Internal_WasActionJustPressed);
	mono_add_internal_call("GameplayCore.Input::Internal_WasActionJustReleased", Internal_WasActionJustReleased);

	mono_add_internal_call("GameplayCore.Input::Internal_GetKeyValue", Internal_GetKeyValue);
	mono_add_internal_call("GameplayCore.Input::Internal_GetAxisValue", Internal_GetAxisValue);

	mono_add_internal_call("GameplayCore.Log::Internal_Log", Internal_Log);
	mono_add_internal_call("GameplayCore.Log::Internal_LogWarning", Internal_LogWarning);
	mono_add_internal_call("GameplayCore.Log::Internal_LogError", Internal_LogError);
	mono_add_internal_call("GameplayCore.Log::Internal_RemoveLogMessage", Internal_RemoveLogMessage);
}

void Engine::SendTimeData() {
	auto time_type = runtime_.GetAssembly().get_type("GameplayCore", "Time");

	float dt = duration<float>(dt_).count();
	delta_time_property_.SetValue(&dt);

	float ellapsed = duration<float>(ellapsed_).count();
	ellapsed_time_property_.SetValue(&ellapsed);
}

void Engine::SendScreenData() {
	auto out_texture = renderer_.GetRenderTargetTexture("outTexture");

	screen_width_property_.SetValue(&out_texture.width);
	screen_height_property_.SetValue(&out_texture.height);
}

void Engine::SendInputData() {
	auto mouse_position = InputManager::getInstance().GetMousePosition();
	DirectX::SimpleMath::Vector2 point(mouse_position.first, mouse_position.second);
	mouse_position_property_.SetValue(&point);
}

#pragma region Renderer

void Engine::Internal_RegisterModel(
	RenderDevice* renderer, 
	size_t id) 
{
	std::vector<ModelVertex> verticies;
	std::vector<uint32_t> indexes;
	size_t primitiveCount = 0;

	ModelMesh model(EPrimitiveType::PRIMITIVETYPE_TRIANGLELIST, 0, verticies, indexes);

	std::string path = "Content\\Cube.obj";
	ModelLoader::LoadObj(path, model);

	renderer->RegisterModel(id, model);

	DirectX::ScratchImage image;
	TextureLoader::LoadWic(L"Content\\Breaks.jpg", image);

	int texture_width = image.GetImage(0, 0, 0)->width;
	int texture_height = image.GetImage(0, 0, 0)->height;
	void* data = image.GetImage(0, 0, 0)->pixels;

	renderer->RegisterTexture(id, texture_width, texture_height, data);
}

void Engine::Internal_DrawModel(
	RenderDevice* renderer,
	size_t id, 
	DirectX::SimpleMath::Matrix model_matrix) 
{
	renderer->DrawModel({ id, id, model_matrix });
}

void Engine::Internal_DrawCurve(
	RenderDevice* renderer, 
	MonoArray* points, 
	DirectX::SimpleMath::Vector3 color,
	DirectX::SimpleMath::Matrix model_matrix) 
{
	mono::mono_array raw_vertices(points);
	MeshData<DebugVertex3D> mesh;
	mesh.pt = PRIMITIVETYPE_LINESTRIP;
	mesh.primitiveCount = raw_vertices.get_length() - 1;

	for (size_t i = 0; i < raw_vertices.get_length(); ++i) {
		auto vertex = raw_vertices.get<DebugVertex3D>(i);
		mesh.vertices.push_back(vertex);
		mesh.indexes.push_back(i);
	}

	DebugDraw3DData debug_draw_data;
	debug_draw_data.mesh = mesh;
	debug_draw_data.color = color;
	debug_draw_data.world = model_matrix;

	renderer->DrawDebug(debug_draw_data);
}

void Engine::Internal_SetViewProjection(
	RenderDevice* renderer,
	float ellapsed,
	DirectX::SimpleMath::Matrix view,
	DirectX::SimpleMath::Matrix projection)
{
	renderer->SetRenderData({ ellapsed, view, projection });
}

#pragma endregion Renderer

#pragma region Physics

JPH::uint32 Engine::Internal_CreateSphereBody(
	JPH::PhysicsSystem* physics_system,
	float radius,
	JPH::Vec3 position,
	JPH::Quat rotation,
	JPH::EMotionType motion_type,
	JPH::uint8 layer) 
{
	JPH::BodyInterface& body_interface = physics_system->GetBodyInterface();
	JPH::SphereShape* sphere_shape = new JPH::SphereShape(radius);
	JPH::BodyCreationSettings body_settings(sphere_shape, position, rotation, motion_type, layer);
	JPH::BodyID body_id = body_interface.CreateAndAddBody(body_settings, JPH::EActivation::DontActivate);
	return body_id.GetIndexAndSequenceNumber();
}

JPH::uint32 Engine::Internal_CreateBoxBody(
	JPH::PhysicsSystem* physics_system,
	JPH::Vec3 half_extent,
	JPH::Vec3 position,
	JPH::Quat rotation,
	JPH::EMotionType motion_type,
	JPH::uint8 layer)
{
	JPH::BodyInterface& body_interface = physics_system->GetBodyInterface();
	JPH::BoxShape* box_shape = new JPH::BoxShape(half_extent);
	JPH::BodyCreationSettings body_settings(box_shape, position, rotation, motion_type, layer);
	JPH::BodyID body_id = body_interface.CreateAndAddBody(body_settings, JPH::EActivation::DontActivate);		
	return body_id.GetIndexAndSequenceNumber();
}

void Engine::Internal_DestroyBody(
	JPH::PhysicsSystem* physics_system, 
	JPH::uint32 id) 
{
	JPH::BodyID body_id(id);
	JPH::BodyInterface& body_interface = physics_system->GetBodyInterface();
	body_interface.RemoveBody(body_id);
	body_interface.DestroyBody(body_id);
}

void Engine::Internal_SetSphereShape(
	JPH::PhysicsSystem* physics_system, 
	JPH::uint32 id, 
	float radius) 
{
	JPH::BodyID body_id(id);
	JPH::BodyInterface& body_interface = physics_system->GetBodyInterface();
	JPH::SphereShape* sphere_shape = new JPH::SphereShape(radius);
	auto activation_mode = body_interface.IsActive(body_id) ? JPH::EActivation::Activate : JPH::EActivation::DontActivate;
	body_interface.SetShape(body_id, sphere_shape, true, activation_mode);
}

void Engine::Internal_SetBoxShape(
	JPH::PhysicsSystem* physics_system, 
	JPH::uint32 id, 
	JPH::Vec3 half_extent) 
{
	JPH::BodyID body_id(id);
	JPH::BodyInterface& body_interface = physics_system->GetBodyInterface();
	JPH::BoxShape* box_shape = new JPH::BoxShape(half_extent);
	auto activation_mode = body_interface.IsActive(body_id) ? JPH::EActivation::Activate : JPH::EActivation::DontActivate;
	body_interface.SetShape(body_id, box_shape, true, activation_mode);
}

void Engine::Internal_SetMotionType(
	JPH::PhysicsSystem* physics_system, 
	JPH::uint32 id, 
	JPH::EMotionType motion_type) 
{
	JPH::BodyID body_id(id);
	JPH::BodyInterface& body_interface = physics_system->GetBodyInterface();
	auto activation_mode = body_interface.IsActive(body_id) ? JPH::EActivation::Activate : JPH::EActivation::DontActivate;
	body_interface.SetMotionType(body_id, motion_type, activation_mode);
}

void Engine::Internal_SetActive(
	JPH::PhysicsSystem* physics_system, 
	JPH::uint32 id, 
	bool is_active) 
{
	JPH::BodyID body_id(id);
	JPH::BodyInterface& body_interface = physics_system->GetBodyInterface();

	if (is_active) {
		body_interface.ActivateBody(body_id);
	} else {
		body_interface.DeactivateBody(body_id);
	}	
}

bool Engine::Internal_IsActive(
	JPH::PhysicsSystem* physics_system, 
	JPH::uint32 id) 
{
	JPH::BodyID body_id(id);
	JPH::BodyInterface& body_interface = physics_system->GetBodyInterface();
	return body_interface.IsActive(body_id);
}

JPH::Vec3 Engine::Internal_GetBodyPosition(
	JPH::PhysicsSystem* physics_system, 
	JPH::uint32 id) 
{
	JPH::BodyID body_id(id);
	JPH::BodyInterface& body_interface = physics_system->GetBodyInterface();
	return body_interface.GetPosition(body_id);
}

void Engine::Internal_SetBodyPosition(
	JPH::PhysicsSystem* physics_system,
	JPH::uint32 id,
	JPH::Vec3 position)
{
	JPH::BodyID body_id(id);
	JPH::BodyInterface& body_interface = physics_system->GetBodyInterface();
	auto activation_mode = body_interface.IsActive(body_id) ? JPH::EActivation::Activate : JPH::EActivation::DontActivate;
	body_interface.SetPosition(body_id, position, activation_mode);
}

JPH::Quat Engine::Internal_GetBodyRotation(
	JPH::PhysicsSystem* physics_system, 
	JPH::uint32 id) 
{
	JPH::BodyID body_id(id);
	JPH::BodyInterface& body_interface = physics_system->GetBodyInterface();
	return body_interface.GetRotation(body_id);
}

void Engine::Internal_SetBodyRotation(
	JPH::PhysicsSystem* physics_system,
	JPH::uint32 id,
	JPH::Quat rotation)
{
	JPH::BodyID body_id(id);
	JPH::BodyInterface& body_interface = physics_system->GetBodyInterface();
	auto activation_mode = body_interface.IsActive(body_id) ? JPH::EActivation::Activate : JPH::EActivation::DontActivate;
	body_interface.SetRotation(body_id, rotation, activation_mode);
}

void Engine::Internal_AddForce(
	JPH::PhysicsSystem* physics_system, 
	JPH::uint32 id, 
	JPH::Vec3 force) 
{
	JPH::BodyID body_id(id);
	JPH::BodyInterface& body_interface = physics_system->GetBodyInterface();
	body_interface.AddForce(body_id, force);
}

#pragma endregion Physics

#pragma region Inputs

bool Engine::Internal_IsPressed(MonoString* key_name) {
	auto raw_string = mono_string_to_utf8(key_name);
	FKey key(raw_string);
	return InputManager::getInstance().GetPlayerInput()->IsPressed(key);
}

bool Engine::Internal_WasJustPressed(MonoString* key_name) {
	auto raw_string = mono_string_to_utf8(key_name);
	FKey key(raw_string);
	return InputManager::getInstance().GetPlayerInput()->WasJustPressed(key);
}

bool Engine::Internal_WasJustReleased(MonoString* key_name) {
	auto raw_string = mono_string_to_utf8(key_name);
	FKey key(raw_string);
	return InputManager::getInstance().GetPlayerInput()->WasJustReleased(key);
}

bool Engine::Internal_IsActionPressed(MonoString* action_name) {
	auto raw_string = mono_string_to_utf8(action_name);
	return InputManager::getInstance().GetPlayerInput()->IsActionPressed(raw_string);
}

bool Engine::Internal_WasActionJustPressed(MonoString* action_name) {
	auto raw_string = mono_string_to_utf8(action_name);
	return InputManager::getInstance().GetPlayerInput()->WasActionJustPressed(raw_string);
}

bool Engine::Internal_WasActionJustReleased(MonoString* action_name) {
	auto raw_string = mono_string_to_utf8(action_name);
	return InputManager::getInstance().GetPlayerInput()->WasActionJustReleased(raw_string);
}

float Engine::Internal_GetKeyValue(MonoString* key_name) {
	auto raw_string = mono_string_to_utf8(key_name);
	FKey key(raw_string);
	return InputManager::getInstance().GetPlayerInput()->GetKeyValue(key);
}

float Engine::Internal_GetAxisValue(MonoString* axis_name) {
	auto raw_string = mono_string_to_utf8(axis_name);
	return InputManager::getInstance().GetPlayerInput()->GetAxisValue(raw_string);
}

#pragma endregion Inputs

void Engine::Internal_RemoveLogMessage(MonoString* guid)
{
	const auto guid_raw_string = mono_string_to_utf8(guid);
	const std::string guid_string(guid_raw_string);
	LogManager::getInstance().OnRemoveViewportPrint(guid_string);
}

void Engine::Internal_Log(MonoString* message, bool should_print_to_screen, bool should_print_to_log, MonoString* guid)
{
	Internal_Log_Implementation(loguru::Verbosity_INFO, message, should_print_to_screen, should_print_to_log, guid);
}

void Engine::Internal_LogWarning(MonoString* message, bool should_print_to_screen, bool should_print_to_log, MonoString* guid)
{
	Internal_Log_Implementation(loguru::Verbosity_WARNING, message, should_print_to_screen, should_print_to_log, guid);
}

void Engine::Internal_LogError(MonoString* message, bool should_print_to_screen, bool should_print_to_log, MonoString* guid)
{
	Internal_Log_Implementation(loguru::Verbosity_ERROR, message, should_print_to_screen, should_print_to_log, guid);
}

void Engine::Internal_Log_Implementation(
	loguru::Verbosity verbosity, 
	MonoString* message, 
	bool should_print_to_screen,
	bool should_print_to_log, 
	MonoString* guid)
{
	if (!should_print_to_log) {
		LogManager::getInstance().SetNextMessageNotBroadcastLog();
	}		

	if (!should_print_to_screen) {
		LogManager::getInstance().SetNextMessageNotBroadcastLogViewport();
	}		

	const auto raw_string = mono_string_to_utf8(message);
	const std::string message_string(raw_string);

	//call log callback
	LogManager::getInstance().Log(verbosity, raw_string);

	const auto guid_raw_string = mono_string_to_utf8(guid);
	const std::string guid_string(guid_raw_string);

	if (should_print_to_screen && !guid_string.empty()) {
		LogManager::getInstance().OnViewportPrint(message_string, verbosity, guid_string);
	}		
}

} // namespace engine
