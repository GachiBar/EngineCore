#include "pch.h"
#include "Engine.h"
#include "ModelLoader.h"
#include "../monowrapper/monopp/mono_method.h"
#include "../monowrapper/monopp/mono_method_invoker.h"
#include "../monowrapper/monopp/mono_property_invoker.h"
#include "../Editor/InputSystem/InputManager.h"

#include <fstream>
#include <iostream>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <algorithm>

#include "GameObject.h"
#include "Scene.h"

namespace engine {

Engine::Engine(const mono::mono_domain& domain, const mono::mono_assembly& assembly)
	: domain_(domain)
	, assembly_(assembly)
	, scene_(nullptr)
{}

Scene* Engine::GetScene() {
	return scene_;
}

void Engine::SetScene(Scene* scene) {
	scene_ = scene;
}

RenderDevice& Engine::GetRenderer()
{
	return renderer_;
}

void Engine::Internal_RegisterModel(RenderDevice* renderer, size_t id) {
	std::vector<ModelVertex> verticies;
	std::vector<uint16_t> indexes;
	size_t primitiveCount = 0;

	ModelData model(verticies, indexes, EPrimitiveType::PRIMITIVETYPE_TRIANGLELIST, 0);

	std::string path = "TestSetup\\Content\\Cube.obj";
	auto m = ModelLoader::LoadObj(path, model);
	renderer->RegisterModel(id, model);
}

void Engine::Internal_DrawModel(RenderDevice* renderer, size_t id, DirectX::SimpleMath::Matrix model_matrix) {
	renderer->DrawModel(id, 0, model_matrix, ModelDefines::MRED);
}

void Engine::Initialize(HWND handle_old, HWND handle_new, UINT width, UINT height) {
	InitRenderer(handle_old, handle_new, static_cast<size_t>(width), static_cast<size_t>(height));

	SetupRendererInternalCalls();
	scene_->Initialize();
}

void Engine::Terminate() {
	scene_->Terminate();
}

void Engine::RunFrame()
{
	using namespace std::chrono;
	using clock = high_resolution_clock;

	auto dt = clock::now() - time_start_;
	ellapsed_ += dt;
	time_start_ = clock::now();
	lag_ += duration_cast<nanoseconds>(dt);

	SendDeltaTime(duration<float>(dt).count());

	while (lag_ >= kTimestep) {
		lag_ -= kTimestep;
		scene_->FixedUpdate();
	}

	scene_->Update();
		
	DirectX::SimpleMath::Vector3 eye = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Vector3 direction = DirectX::SimpleMath::Vector3::UnitZ;
	auto view = DirectX::SimpleMath::Matrix::CreateLookAt(eye, direction, DirectX::SimpleMath::Vector3::Up);
	auto projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		DirectX::XM_PIDIV2,
		800.0f / 600,
		0.1f,
		100.0f);

	renderer_.SetRenderData({
		duration<float>(ellapsed_).count(),
		view,
		projection
	});
}

bool Engine::ProcessMessages(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	return renderer_.ProcessMessages(hwnd, msg, wparam, lparam);
}

void Engine::InitRenderer(HWND handle_old, HWND handle_new, size_t width, size_t height) {
	renderer_.InitDevice({ handle_old, handle_new, width, height });
	renderer_.InitShaders("..\\DX11RenderEngine\\GachiRenderSystem\\Shaders\\");
}

void Engine::SetupRendererInternalCalls() {
	mono_add_internal_call("GameplayCore.EngineApi.Render::Internal_RegisterModel", Internal_RegisterModel);
	mono_add_internal_call("GameplayCore.EngineApi.Render::Internal_DrawModel", Internal_DrawModel);

	auto renderer_type = assembly_.get_type("GameplayCore.EngineApi", "Render");
	auto renderer_property = renderer_type.get_property("Renderer");
	mono::mono_property_invoker renderer_property_invoker(renderer_property);
	renderer_property_invoker.set_value(&renderer_);
}

void Engine::SendDeltaTime(float dt) {
	auto time_type = assembly_.get_type("GameplayCore", "Time");
	auto delta_time_property = time_type.get_property("DeltaTime");
	mono::mono_property_invoker delta_time_property_invoker(delta_time_property);
	delta_time_property_invoker.set_value(&dt);
}

} // namespace engine
