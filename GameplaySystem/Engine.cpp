#include "pch.h"
#include "Engine.h"
#include "ModelLoader.h"
#include "GameObject.h"
#include "Scene.h"
#include "TextureLoader.h"
#include "../InputSystem/InputManager.h"
#include "../monowrapper/monopp/mono_method.h"
#include "../monowrapper/monopp/mono_method_invoker.h"
#include "../monowrapper/monopp/mono_property_invoker.h"

#include <fstream>
#include <iostream>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <algorithm>

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

Engine::Engine(const mono::mono_domain& domain, const mono::mono_assembly& assembly)
	: temp_allocator_(10 * 1024 * 1024)
	, scene_(nullptr)
	, domain_(domain)
	, assembly_(assembly)
	, renderer_property_(GetProperty("GameplayCore.EngineApi", "Render", "Renderer"))
	, delta_time_property_(GetProperty("GameplayCore", "Time", "DeltaTime"))
	, ellapsed_time_property_(GetProperty("GameplayCore", "Time", "EllapsedTime"))
	, screen_width_property_(GetProperty("GameplayCore", "Screen", "Width"))
	, screen_height_property_(GetProperty("GameplayCore", "Screen", "Height"))
	, mouse_position_property_(GetProperty("GameplayCore", "Input", "MousePosition"))
{}

void Engine::Initialize(HWND handle_old, HWND handle_new, UINT width, UINT height) {
	InitRenderer(handle_old, handle_new, static_cast<size_t>(width), static_cast<size_t>(height));
	InitPhysicsSystem();
	SetupRendererInternalCalls();
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

void Engine::InitRenderer(HWND handle_old, HWND handle_new, size_t width, size_t height) {
	WindowSettings window_settings{};
	window_settings.windowWidth = width;
	window_settings.windowHeight = height;
	window_settings.viewportWidth = width;
	window_settings.viewportHeight = height;
	
	RenderEngineCoreSettings render_engine_core_settings{};
	render_engine_core_settings.hWnd1 = handle_old;
	render_engine_core_settings.hWnd2 = handle_new;
	render_engine_core_settings.windowSettings = window_settings;

	renderer_.CreateDevice(render_engine_core_settings);
	renderer_.InitDevice({ "..\\DX11RenderEngine\\GachiRenderSystem\\Shaders\\" });
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

void Engine::SetupRendererInternalCalls() {
	mono_add_internal_call("GameplayCore.EngineApi.Render::Internal_RegisterModel", Internal_RegisterModel);
	mono_add_internal_call("GameplayCore.EngineApi.Render::Internal_DrawModel", Internal_DrawModel);
	mono_add_internal_call("GameplayCore.EngineApi.Render::Internal_SetViewProjection", Internal_SetViewProjection);

	renderer_property_.set_value(&renderer_);
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

mono::mono_property Engine::GetProperty(std::string name_space, std::string klass, std::string property) {
	return assembly_.get_type(name_space, klass).get_property(property);
}

void Engine::SendTimeData() {
	auto time_type = assembly_.get_type("GameplayCore", "Time");

	float dt = duration<float>(dt_).count();
	delta_time_property_.set_value(&dt);

	float ellapsed = duration<float>(ellapsed_).count();
	ellapsed_time_property_.set_value(&ellapsed);
}

void Engine::SendScreenData() {
	auto out_texture = renderer_.GetRenderTargetTexture("outTexture");

	screen_width_property_.set_value(&out_texture.width);
	screen_height_property_.set_value(&out_texture.height);
}

void Engine::SendInputData() {
	auto mouse_position = InputManager::getInstance().GetMousePosition();
	DirectX::SimpleMath::Vector2 point(mouse_position.first, mouse_position.second);
	mouse_position_property_.set_value(&point);
}

void Engine::Internal_RegisterModel(RenderDevice* renderer, size_t id) {
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

void Engine::Internal_DrawModel(RenderDevice* renderer, size_t id, DirectX::SimpleMath::Matrix model_matrix) {
	renderer->DrawModel({ id, id, model_matrix });
}

void Engine::Internal_SetViewProjection(
	RenderDevice* renderer,
	float ellapsed,
	DirectX::SimpleMath::Matrix view,
	DirectX::SimpleMath::Matrix projection)
{
	renderer->SetRenderData({ ellapsed, view, projection });
}

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

} // namespace engine
