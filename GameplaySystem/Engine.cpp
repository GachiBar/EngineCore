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
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Collision/NarrowPhaseQuery.h>
#include <Jolt/Physics/Collision/RayCast.h>
#include <Jolt/Physics/Collision/CastResult.h>

#include <fstream>
#include <iostream>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <algorithm>

#include "../Editor/Resources/TransferMaterial.h"

JPH_SUPPRESS_WARNINGS

namespace engine {

const float Engine::kDt = 16.0f / 1000;
const float Engine::kAiDt = 16.0f * 20.0f / 1000;
const nanoseconds Engine::kTimestep = duration_cast<nanoseconds>(milliseconds(16));
const nanoseconds Engine::kAiTimestep = duration_cast<nanoseconds>(milliseconds(16 * 20));
Engine* Engine::engine_ = nullptr;

bool Engine::IsRunning() {
	return is_running_;
}

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
	, collision_enter_method_(runtime_.GetType(Types::kPhysicsApi).GetMethod("CollisionEnter", 2))
	, collision_stay_method_(runtime_.GetType(Types::kPhysicsApi).GetMethod("CollisionStay", 2))
	, collision_exit_method_(runtime_.GetType(Types::kPhysicsApi).GetMethod("CollisionExit", 2))
	, is_running_(false)
{
	engine_ = this;
}

void Engine::Initialize(HWND handle, UINT width, UINT height) {
	InitRenderer(handle, static_cast<size_t>(width), static_cast<size_t>(height));
	InitPhysicsSystem();
	SetupRendererInternalCalls();
	SetupPhysicsInternalCalls();
	SetupInputInternalCalls();
	SetupLogInternalCalls();
}

void Engine::Terminate() {
	scene_->Terminate();

	delete JPH::Factory::sInstance;
	JPH::Factory::sInstance = nullptr;
}

void Engine::Stop() {
	RunFrame();
	is_running_ = false;
}

void Engine::Start() {
	using clock = high_resolution_clock;

	time_start_ = clock::now();
	is_running_ = true;
}

void Engine::RunFrame() {
	using clock = high_resolution_clock;

	dt_ = clock::now() - time_start_;
	ellapsed_ += dt_;
	time_start_ = clock::now();
	lag_ += duration_cast<nanoseconds>(dt_);
	ai_lag_ += duration_cast<nanoseconds>(dt_);

	SendTimeData();
	SendScreenData();
	SendInputData();

	while (lag_ >= kTimestep) 	{
		lag_ -= kTimestep;
		scene_->FixedUpdate();
		physics_system_.Update(kDt, kCollisionSteps, kIntegrationSubSteps, &temp_allocator_, &job_system_);
		SendCollisions();
	}

	while (ai_lag_ >= kAiTimestep) {
		ai_lag_ -= kAiTimestep;
		scene_->UpdateAI();
	}

	scene_->Update();
}

void Engine::BeginRender() {
	renderer_.BeginFrame();	
}

void Engine::Render() {
	scene_->Render();
}

void Engine::EditorRender() {
	scene_->EditorRender();
}

void Engine::EndRender() {
	while (!renderer_.EndFrame()) {
		renderer_.ReloadShaders();
		renderer_.BeginFrame();
	};
}

void Engine::Present()
{
	renderer_.Present();
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

	physics_system_.SetContactListener(&contact_listener_);
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
	
	OpaqueMesh model{ EPrimitiveType::PRIMITIVETYPE_TRIANGLELIST };

	// Stool
	std::string path = "Content\\Stool.obj";
	ModelLoader::LoadObj(path, model);

	renderer_.RegisterModel(1, model);

	DirectX::ScratchImage stool_image;
	TextureLoader::LoadWic(L"Content\\Stool.jpg", stool_image);

	int texture_width = stool_image.GetImage(0, 0, 0)->width;
	int texture_height = stool_image.GetImage(0, 0, 0)->height;
	void* data = stool_image.GetImage(0, 0, 0)->pixels;

	renderer_.RegisterTexture(1, texture_width, texture_height, data);

	// Cube
	std::string cube_path = "Content\\Cube.obj";
	ModelLoader::LoadObj(cube_path, model);

	renderer_.RegisterModel(2, model);

	DirectX::ScratchImage cube_image;
	TextureLoader::LoadWic(L"Content\\Breaks.jpg", cube_image);

	texture_width = cube_image.GetImage(0, 0, 0)->width;
	texture_height = cube_image.GetImage(0, 0, 0)->height;
	data = cube_image.GetImage(0, 0, 0)->pixels;

	renderer_.RegisterTexture(2, texture_width, texture_height, data);
}

void Engine::SetupRendererInternalCalls() {
	mono_add_internal_call("GameplayCore.EngineApi.RenderApi::Internal_RegisterModel", Internal_RegisterModel);
	mono_add_internal_call("GameplayCore.EngineApi.RenderApi::Internal_RegisterTexture", Internal_RegisterTexture);
	mono_add_internal_call("GameplayCore.EngineApi.RenderApi::Internal_DrawModel", Internal_DrawModel);
	mono_add_internal_call("GameplayCore.EngineApi.RenderApi::Internal_DrawDirectionalLight", Internal_DrawDirectionalLight);
	mono_add_internal_call("GameplayCore.EngineApi.RenderApi::Internal_DrawCurve", Internal_DrawCurve);
	mono_add_internal_call("GameplayCore.EngineApi.RenderApi::Internal_SetViewProjection", Internal_SetViewProjection);
	
	mono_add_internal_call("GameplayCore.EngineApi.RenderApi::Internal_IsIdUsed", Internal_IsIdUsed);
	mono_add_internal_call("GameplayCore.EngineApi.RenderApi::Internal_IsMeshIdUsed", Internal_IsMeshIdUsed);
	mono_add_internal_call("GameplayCore.EngineApi.RenderApi::Internal_IsTextureIdUsed", Internal_IsTextureIdUsed);
	
	mono_add_internal_call("GameplayCore.EngineApi.RenderApi::Internal_PullMaterial", Internal_PullMaterial);
	mono_add_internal_call("GameplayCore.EngineApi.RenderApi::Internal_CommitMaterial", Internal_CommitMaterial);
	mono_add_internal_call("GameplayCore.EngineApi.RenderApi::Internal_ContainsMaterialId", Internal_ContainsMaterialId);

	renderer_property_.SetValue(&renderer_);
}

void Engine::SetupPhysicsInternalCalls() {
	mono_add_internal_call("GameplayCore.EngineApi.PhysicsApi::Internal_CreateBody", Internal_CreateBody);
	mono_add_internal_call("GameplayCore.EngineApi.PhysicsApi::Internal_DestroyBody", Internal_DestroyBody);
	mono_add_internal_call("GameplayCore.EngineApi.PhysicsApi::Internal_SetCollisionLayer", Internal_SetCollisionLayer);
	mono_add_internal_call("GameplayCore.EngineApi.PhysicsApi::Internal_SetEmptyShape", Internal_SetEmptyShape);
	mono_add_internal_call("GameplayCore.EngineApi.PhysicsApi::Internal_SetSphereShape", Internal_SetSphereShape);
	mono_add_internal_call("GameplayCore.EngineApi.PhysicsApi::Internal_SetBoxShape", Internal_SetBoxShape);
	mono_add_internal_call("GameplayCore.EngineApi.PhysicsApi::Internal_SetCapsuleShape", Internal_SetCapsuleShape);
	mono_add_internal_call("GameplayCore.EngineApi.PhysicsApi::Internal_SetMotionType", Internal_SetMotionType);
	mono_add_internal_call("GameplayCore.EngineApi.PhysicsApi::Internal_SetActive", Internal_SetActive);
	mono_add_internal_call("GameplayCore.EngineApi.PhysicsApi::Internal_IsActive", Internal_IsActive);
	mono_add_internal_call("GameplayCore.EngineApi.PhysicsApi::Internal_GetBodyPosition", Internal_GetBodyPosition);
	mono_add_internal_call("GameplayCore.EngineApi.PhysicsApi::Internal_SetBodyPosition", Internal_SetBodyPosition);
	mono_add_internal_call("GameplayCore.EngineApi.PhysicsApi::Internal_GetBodyRotation", Internal_GetBodyRotation);
	mono_add_internal_call("GameplayCore.EngineApi.PhysicsApi::Internal_SetBodyRotation", Internal_SetBodyRotation);
	mono_add_internal_call("GameplayCore.EngineApi.PhysicsApi::Internal_FreezeRotation", Internal_FreezeRotation);
	mono_add_internal_call("GameplayCore.EngineApi.PhysicsApi::Internal_SetMass", Internal_SetMass);
	mono_add_internal_call("GameplayCore.EngineApi.PhysicsApi::Internal_AddForce", Internal_AddForce);
	mono_add_internal_call("GameplayCore.EngineApi.PhysicsApi::Internal_AddImpulse", Internal_AddImpulse);
	mono_add_internal_call("GameplayCore.EngineApi.PhysicsApi::Internal_GetLinearVelocity", Internal_GetLinearVelocity);
	mono_add_internal_call("GameplayCore.EngineApi.PhysicsApi::Internal_SetLinearVelocity", Internal_SetLinearVelocity);	
	mono_add_internal_call("GameplayCore.EngineApi.PhysicsApi::Internal_CastRay", Internal_CastRay);

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
}

void Engine::SetupLogInternalCalls() {
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

void Engine::SendCollisions() {
	void* params[2];

	while (contact_listener_.IsAnyCollisionsEnter()) {
		auto pair = contact_listener_.PopCollisionsEnter();
		params[0] = &pair.body_id_1;
		params[1] = &pair.body_id_2;

		collision_enter_method_.Invoke(params);
	}

	while (contact_listener_.IsAnyCollisionsStay()) {
		auto pair = contact_listener_.PopCollisionsStay();
		params[0] = &pair.body_id_1;
		params[1] = &pair.body_id_2;

		collision_stay_method_.Invoke(params);
	}

	while (contact_listener_.IsAnyCollisionsExit()) {
		auto pair = contact_listener_.PopCollisionsExit();
		params[0] = &pair.body_id_1;
		params[1] = &pair.body_id_2;

		collision_exit_method_.Invoke(params);
	}
}

#pragma region Renderer

void Engine::Internal_RegisterModel(RenderDevice* renderer, size_t id, MonoString* path)
{
	const auto raw_string = mono_string_to_utf8(path);
	const std::string path_string(raw_string);
	mono_free(raw_string);

	std::vector<OpaqueModelVertex> verticies;
	std::vector<uint32_t> indexes;
	OpaqueMesh model{ EPrimitiveType::PRIMITIVETYPE_TRIANGLELIST, 0, verticies, indexes };
	
	ModelLoader::LoadObj(path_string, model);
	renderer->RegisterModel(id, model);
}

void Engine::Internal_RegisterTexture(RenderDevice* renderer, size_t id, MonoString* path)
{
	const auto raw_string = mono_string_to_utf8(path);
	const std::string path_string(raw_string);
	mono_free(raw_string);

	DirectX::ScratchImage image;
	std::wstring wpath(path_string.begin(), path_string.end());
	TextureLoader::LoadWic(wpath, image);

	const int texture_width = image.GetImage(0, 0, 0)->width;
	const int texture_height = image.GetImage(0, 0, 0)->height;
	void* data = image.GetImage(0, 0, 0)->pixels;

	renderer->RegisterTexture(id, texture_width, texture_height, data);
}

void Engine::Internal_DrawModel(
	RenderDevice* renderer, 
	size_t id, 
	size_t material_id, 
	DirectX::SimpleMath::Matrix model_matrix) 
{
	MaterialData material =  GetMaterialData(material_id);
	OpaqueModelDrawData opaque_model_draw_data{
		id, model_matrix, model_matrix, 1.0f, 1.0f, material, {}
	};
	
	renderer->DrawOpaqueModel(opaque_model_draw_data);
}

void Engine::Internal_DrawDirectionalLight(
	RenderDevice* renderer,
	DirectX::SimpleMath::Vector3 direction,
	float itencity,
	DirectX::SimpleMath::Color color) 
{
	DirectionalLight directional_light(direction, itencity, color);
	LightDrawData light_draw_data{
		DirectX::SimpleMath::Matrix::Identity,
		directional_light,
	};

	renderer->DrawLight(light_draw_data);
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
	renderer->SetRenderData({ ellapsed, view, projection, {0,1,0,0} });
}

bool Engine::Internal_IsIdUsed(RenderDevice* renderer, size_t id)
{
	return renderer->WasIdUsed(id);
}

bool Engine::Internal_IsMeshIdUsed(RenderDevice* renderer, size_t id)
{
	return renderer->IsMeshIdUsed(id);
}

bool Engine::Internal_IsTextureIdUsed(RenderDevice* renderer, size_t id)
{
	return renderer->IsTextureIdUsed(id);
}

MaterialData Engine::GetMaterialData(size_t id)
{
	if(engine_->_materials.contains(id))
		return engine_->_materials[id];
	
	return MaterialData();
}
	
TransferMaterial Engine::Internal_PullMaterial(size_t id)
{
	if(engine_->_materials.contains(id))
		return TransferMaterial(engine_->_materials[id]);
	
	return {};
}
	
void Engine::Internal_CommitMaterial(size_t id, TransferMaterial data)
{
	engine_->_materials[id] = data.Convert();
}

bool Engine::Internal_ContainsMaterialId(size_t id)
{
	return engine_->_materials.contains(id);
}

#pragma endregion Renderer

#pragma region Physics

JPH::uint32 Engine::Internal_CreateBody(
	JPH::PhysicsSystem* physics_system,
	JPH::Vec3 position,
	JPH::Quat rotation,
	JPH::EMotionType motion_type) 
{
	JPH::BodyInterface& body_interface = physics_system->GetBodyInterface();
	JPH::SphereShape* sphere_shape = new JPH::SphereShape(FLT_EPSILON);
	JPH::BodyCreationSettings body_settings(sphere_shape, position, rotation, motion_type, CollisionLayers::kNoCollision);
	body_settings.mAllowDynamicOrKinematic = true;
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

void Engine::Internal_SetCollisionLayer(
	JPH::PhysicsSystem* physics_system,
	JPH::uint32 id,
	JPH::uint8 layer)
{
	JPH::BodyID body_id(id);
	JPH::BodyInterface& body_interface = physics_system->GetBodyInterface();
	body_interface.SetObjectLayer(body_id, layer);
}

void Engine::Internal_SetEmptyShape(
	JPH::PhysicsSystem* physics_system,
	JPH::uint32 id) 
{
	JPH::BodyID body_id(id);
	JPH::BodyInterface& body_interface = physics_system->GetBodyInterface();
	JPH::SphereShape* sphere_shape = new JPH::SphereShape(FLT_EPSILON);
	body_interface.SetShape(body_id, sphere_shape, false, JPH::EActivation::DontActivate);
}

void Engine::Internal_SetSphereShape(
	JPH::PhysicsSystem* physics_system, 
	JPH::uint32 id, 
	float radius)
{
	JPH::BodyID body_id(id);
	JPH::BodyInterface& body_interface = physics_system->GetBodyInterface();
	JPH::SphereShape* sphere_shape = new JPH::SphereShape(radius);
	body_interface.SetShape(body_id, sphere_shape, false, JPH::EActivation::DontActivate);
}

void Engine::Internal_SetBoxShape(
	JPH::PhysicsSystem* physics_system, 
	JPH::uint32 id, 
	JPH::Vec3 half_extent) 
{
	JPH::BodyID body_id(id);
	JPH::BodyInterface& body_interface = physics_system->GetBodyInterface();
	JPH::BoxShape* box_shape = new JPH::BoxShape(half_extent);
	body_interface.SetShape(body_id, box_shape, false, JPH::EActivation::DontActivate);
}

void Engine::Internal_SetCapsuleShape(
	JPH::PhysicsSystem* physics_system,
	JPH::uint32 id,
	float half_height,
	float radius) 
{
	JPH::BodyID body_id(id);
	JPH::BodyInterface& body_interface = physics_system->GetBodyInterface();
	JPH::CapsuleShape* capsule_shape = new JPH::CapsuleShape(half_height, radius);
	body_interface.SetShape(body_id, capsule_shape, false, JPH::EActivation::DontActivate);
}

void Engine::Internal_SetMotionType(
	JPH::PhysicsSystem* physics_system, 
	JPH::uint32 id, 
	JPH::EMotionType motion_type) 
{
	JPH::BodyID body_id(id);
	JPH::BodyInterface& body_interface = physics_system->GetBodyInterface();
	body_interface.SetMotionType(body_id, motion_type, JPH::EActivation::DontActivate);
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
	body_interface.SetPosition(body_id, position, JPH::EActivation::DontActivate);
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
	body_interface.SetRotation(body_id, rotation.Normalized(), JPH::EActivation::DontActivate);
}

void Engine::Internal_FreezeRotation(
	JPH::PhysicsSystem* physics_system,
	JPH::uint32 id)
{
	JPH::BodyID body_id(id);
	const JPH::BodyLockInterface& body_lock_interface = physics_system->GetBodyLockInterface();
	JPH::BodyLockWrite lock(body_lock_interface, body_id);

	if (!lock.Succeeded()) {
		return;
	}

	JPH::Body& body = lock.GetBody();
	JPH::MotionProperties* motion_properties = body.GetMotionProperties();
	motion_properties->SetInverseInertia(JPH::Vec3::sZero(), JPH::Quat::sIdentity());
}

void Engine::Internal_SetMass(
	JPH::PhysicsSystem* physics_system,
	JPH::uint32 id,
	float mass)
{
	JPH::BodyID body_id(id);
	const JPH::BodyLockInterface& body_lock_interface = physics_system->GetBodyLockInterface();
	JPH::BodyLockWrite lock(body_lock_interface, body_id);

	if (!lock.Succeeded()) {
		return;
	}

	JPH::MassProperties mass_properties;
	mass_properties.ScaleToMass(mass);

	JPH::Body& body = lock.GetBody();
	JPH::MotionProperties* motion_properties = body.GetMotionProperties();
	motion_properties->SetMassProperties(mass_properties);
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

void Engine::Internal_AddImpulse(
	JPH::PhysicsSystem* physics_system,
	JPH::uint32 id,
	JPH::Vec3 impulse)
{
	JPH::BodyID body_id(id);
	JPH::BodyInterface& body_interface = physics_system->GetBodyInterface();
	body_interface.AddImpulse(body_id, impulse);
}

JPH::Vec3 Engine::Internal_GetLinearVelocity(
	JPH::PhysicsSystem* physics_system,
	JPH::uint32 id)
{
	JPH::BodyID body_id(id);
	JPH::BodyInterface& body_interface = physics_system->GetBodyInterface();
	return body_interface.GetLinearVelocity(body_id);
}

void Engine::Internal_SetLinearVelocity(
	JPH::PhysicsSystem* physics_system,
	JPH::uint32 id,
	JPH::Vec3 velocity)
{
	JPH::BodyID body_id(id);
	JPH::BodyInterface& body_interface = physics_system->GetBodyInterface();
	body_interface.SetLinearVelocity(body_id, velocity);
}

bool Engine::Internal_CastRay(
	JPH::PhysicsSystem* physics_system,
	JPH::Vec3 origin,
	JPH::Vec3 direction,
	JPH::uint32& body_id)
{
	// TODO: return hit result;
	// TODO: what is broad phase and object layers?
	const JPH::NarrowPhaseQuery& narrow_phase_query = physics_system->GetNarrowPhaseQuery();

	JPH::RayCast ray{ origin, direction };
	JPH::RayCastResult hit;
	JPH::BroadPhaseLayerFilter bp_layer_filter;
	JPH::ObjectLayerFilter object_layer_filter;
	JPH::BodyFilter body_filter;
		
	if (narrow_phase_query.CastRay(ray, hit, bp_layer_filter, object_layer_filter, body_filter)) {
		body_id = hit.mBodyID.GetIndexAndSequenceNumber();
		return true;
	}

	return false;
}

#pragma endregion Physics

#pragma region Inputs

bool Engine::Internal_IsPressed(MonoString* key_name) {
	auto raw_string = mono_string_to_utf8(key_name);
	FKey key(raw_string);
	mono_free(raw_string);
	return InputManager::getInstance().GetPlayerInput()->IsPressed(key);
}

bool Engine::Internal_WasJustPressed(MonoString* key_name) {
	auto raw_string = mono_string_to_utf8(key_name);
	FKey key(raw_string);
	mono_free(raw_string);
	return InputManager::getInstance().GetPlayerInput()->WasJustPressed(key);
}

bool Engine::Internal_WasJustReleased(MonoString* key_name) {
	auto raw_string = mono_string_to_utf8(key_name);
	FKey key(raw_string);
	mono_free(raw_string);
	return InputManager::getInstance().GetPlayerInput()->WasJustReleased(key);
}

bool Engine::Internal_IsActionPressed(MonoString* action_name) {
	auto raw_string = mono_string_to_utf8(action_name);
	std::string action(raw_string);
	mono_free(raw_string);
	return InputManager::getInstance().GetPlayerInput()->IsActionPressed(action);
}

bool Engine::Internal_WasActionJustPressed(MonoString* action_name) {
	auto raw_string = mono_string_to_utf8(action_name);
	std::string action(raw_string);
	mono_free(action_name);
	return InputManager::getInstance().GetPlayerInput()->WasActionJustPressed(action);
}

bool Engine::Internal_WasActionJustReleased(MonoString* action_name) {
	auto raw_string = mono_string_to_utf8(action_name);
	std::string action(raw_string);
	mono_free(raw_string);
	return InputManager::getInstance().GetPlayerInput()->WasActionJustReleased(action);
}

float Engine::Internal_GetKeyValue(MonoString* key_name) {
	auto raw_string = mono_string_to_utf8(key_name);
	FKey key(raw_string);
	mono_free(raw_string);
	return InputManager::getInstance().GetPlayerInput()->GetKeyValue(key);
}

float Engine::Internal_GetAxisValue(MonoString* axis_name) {
	auto raw_string = mono_string_to_utf8(axis_name);
	std::string axis(raw_string);
	mono_free(raw_string);
	return InputManager::getInstance().GetPlayerInput()->GetAxisValue(axis);
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
	mono_free(raw_string);

	//call log callback
	LogManager::getInstance().Log(verbosity, raw_string);

	const auto guid_raw_string = mono_string_to_utf8(guid);
	const std::string guid_string(guid_raw_string);
	mono_free(guid_raw_string);

	if (should_print_to_screen && !guid_string.empty()) {
		LogManager::getInstance().OnViewportPrint(message_string, verbosity, guid_string);
	}		
}

} // namespace engine
