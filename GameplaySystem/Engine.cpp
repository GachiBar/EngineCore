#include "pch.h"
#include "Engine.h"
#include "ModelLoader.h"
#include "GameObject.h"
#include "Scene.h"
#include "TextureLoader.h"
#include "../monowrapper/monopp/mono_method.h"
#include "../monowrapper/monopp/mono_method_invoker.h"
#include "../monowrapper/monopp/mono_property_invoker.h"

#include <fstream>
#include <iostream>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <algorithm>

namespace engine {

DirectX::SimpleMath::Matrix Engine::m_projection{};
DirectX::SimpleMath::Matrix Engine::m_view{};

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
	: domain_(domain)
	, assembly_(assembly)
	, renderer_property_(GetProperty("GameplayCore.EngineApi", "Render", "Renderer"))
	, delta_time_property_(GetProperty("GameplayCore", "Time", "DeltaTime"))
	, ellapsed_time_property_(GetProperty("GameplayCore", "Time", "EllapsedTime"))
	, screen_width_property_(GetProperty("GameplayCore", "Screen", "Width"))
	, screen_height_property_(GetProperty("GameplayCore", "Screen", "Height"))
	, scene_(nullptr)
{}

void Engine::Internal_RegisterModel(RenderDevice* renderer, size_t id) {
	std::vector<ModelVertex> verticies;
	std::vector<uint16_t> indexes;
	size_t primitiveCount = 0;

	ModelData model(verticies, indexes, EPrimitiveType::PRIMITIVETYPE_TRIANGLELIST, 0);

	std::string path = "Content\\Cube.obj";
	ModelLoader::LoadObj(path, model);

	renderer->RegisterModel(id, model);

	DirectX::ScratchImage image;
	TextureLoader::LoadWic(L"Content\\Breaks.jpg", image);

	renderer->RegisterTexture(
		id, 
		image.GetImage(0, 0, 0)->width, 
		image.GetImage(0, 0, 0)->height,
		image.GetImage(0, 0, 0)->pixels, 
		false);
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
	m_view = view;
	m_projection = projection;
	renderer->SetRenderData({ ellapsed, view, projection });
}

void Engine::Initialize(HWND handle_old, HWND handle_new, UINT width, UINT height) {
	InitRenderer(handle_old, handle_new, static_cast<size_t>(width), static_cast<size_t>(height));
	SetupRendererInternalCalls();
	scene_->Initialize();
}

void Engine::Terminate() {
	scene_->Terminate();
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

DirectX::SimpleMath::Matrix& Engine::GetViewMatrix() {
	return m_view;
}

DirectX::SimpleMath::Matrix& Engine::GetProjectionMatrix() {
	return m_projection;
}

void Engine::InitRenderer(HWND handle_old, HWND handle_new, size_t width, size_t height) {
	renderer_.CreateDevice(
	{
		handle_old,
		handle_new,
		{
			width,
			height,
			width,
			height
		}
	});
	renderer_.InitDevice({ "..\\DX11RenderEngine\\GachiRenderSystem\\Shaders\\" });
}

mono::mono_property Engine::GetProperty(std::string name_space, std::string klass, std::string property) {
	return assembly_.get_type(name_space, klass).get_property(property);
}

void Engine::SetupRendererInternalCalls() {
	mono_add_internal_call("GameplayCore.EngineApi.Render::Internal_RegisterModel", Internal_RegisterModel);
	mono_add_internal_call("GameplayCore.EngineApi.Render::Internal_DrawModel", Internal_DrawModel);
	mono_add_internal_call("GameplayCore.EngineApi.Render::Internal_SetViewProjection", Internal_SetViewProjection);

	renderer_property_.set_value(&renderer_);
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

} // namespace engine
