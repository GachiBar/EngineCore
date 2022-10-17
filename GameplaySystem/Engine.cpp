#include "pch.h"
#include "Engine.h"
#include "CSharpDomain.h"
#include "CSharpAssembly.h"
#include "CSharpObject.h"

#include <fstream>
#include <iostream>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include "../Editor/InputSystem/InputManager.h"
#include <algorithm>

#include "string_view"
#include "MathematicsInternals.h"

namespace engine {

RenderDevice* Engine::current_device_ = nullptr;

CSharpObject* Engine::GetScene() {
	return scene_;
}

void Engine::SetScene(CSharpObject* scene) {
	scene_ = scene;
}

RenderDevice& Engine::GetRenderer()
{
	return renderer_;
}

void Engine::Internal_RegisterModel(size_t id) {
	current_device_->RegisterModel(id, {
		{
			{{ -0.25, -0.25, 0 }, {},{}},
			{{  0.0 ,  0.25, 0 }, {},{}},
			{{  0.25, -0.25, 0 }, {},{}},			
		},
		{0,1,2},
		EPrimitiveType::PRIMITIVETYPE_TRIANGLELIST,
		1
	});
}

void Engine::Internal_DrawModel(size_t id, DirectX::SimpleMath::Matrix model_matrix) {
	current_device_->DrawModel(id, 0, model_matrix, ModelDefines::MRED);
}

void Engine::Initialize(HWND handle_old, HWND handle_new, UINT width, UINT height) {
	InitRenderer(handle_old, handle_new, static_cast<size_t>(width), static_cast<size_t>(height));

	current_device_ = &renderer_;

	mono_add_internal_call("GameplayCore.EngineApi.Render::Internal_RegisterModel", Internal_RegisterModel);
	mono_add_internal_call("GameplayCore.EngineApi.Render::Internal_DrawModel", Internal_DrawModel);

	AddMathematicsInternalCalls();	

	scene_->CallMethod("Initialize");
}

void Engine::Terminate() {
	scene_->CallMethod("Terminate");
}

void Engine::RunFrame()
{
	using namespace std::chrono;
	using clock = high_resolution_clock;

	auto dt = clock::now() - time_start_;
	ellapsed_ += dt;
	time_start_ = clock::now();
	lag_ += duration_cast<nanoseconds>(dt);

	while (lag_ >= kTimestep) {
		lag_ -= kTimestep;
		scene_->CallMethod("FixedUpdate");
	}

	scene_->CallMethod("Update");

	renderer_.SetRenderData({
		duration<float>(ellapsed_).count(),
		matrix::Identity,
		matrix::Identity
	});

	/*
	renderer_.BeginFrame();
	
	renderer_.SetRenderData({});

	//scene_->CallMethod("Render");

	while (!renderer_.Present()) {
		renderer_.EndFrame();
	}

	renderer_.EndFrame();
	*/
}

bool Engine::ProcessMessages(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	return renderer_.ProcessMessages(hwnd, msg, wparam, lparam);
}

void Engine::InitRenderer(HWND handle_old, HWND handle_new, size_t width, size_t height) {
	renderer_.InitDevice({ handle_old, handle_new, width, height });
	renderer_.InitShaders("..\\..\\DX11RenderEngine\\DX11RenderEngine\\Shaders\\");
}

} // namespace engine
