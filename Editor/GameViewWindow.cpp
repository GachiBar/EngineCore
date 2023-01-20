#include "GameViewWindow.h"
#include "InputManager.h"
#include "libs/MathUtility.h"
#include "libs/imgui_sugar.hpp"
#include "../GameplaySystem/Component.h"
#include <iostream>

std::shared_ptr<engine::GameObject> GameViewWindow::GetGameObject()
{
	return game_object;
}

void GameViewWindow::SetGameObject(std::shared_ptr<engine::GameObject> gameObject)
{
	game_object = gameObject;

	if (game_object != nullptr) 
	{
		transform_component = game_object->GetComponent(engine::Types::kTransformComponent);
	}
}

GameViewWindow::GameViewWindow(RenderDevice& renderer)
	: renderer(renderer)
	, selected_render_target("outTexture")
	, texture(nullptr)
	, is_playing(false)
	, is_in_focus(false)
	, is_camera_input_mode(false)
	, last_cursor_position{}
{}

void GameViewWindow::Update(float dt)
{
	if (IsInCameraEditorInputMode())
	{
		editor_camera.Tick(dt);
	}
	else if (is_in_focus)
	{
		UpdateGuizmo();
	}

	if (!is_playing)
	{
		SendCameraData(dt);
	}
}

void GameViewWindow::Draw()
{
	ImGuiWindowFlags windowFlags = 
		ImGuiWindowFlags_NoScrollbar | 
		ImGuiWindowFlags_NoScrollWithMouse | 
		ImGuiWindowFlags_MenuBar;

	with_Window("Game Viewport", nullptr, windowFlags)
	{
		current_window_size = ImGui::GetWindowSize();
		DrawMenu();
		DrawViewport();

		if(!is_playing)
		{
			DrawRenderTargetSelector();
		}
	}
}

void GameViewWindow::OnResizeViewport(int32 InWidth, int32 InHeight)
{
	texture = renderer.GetRenderTargetTexture(selected_render_target.c_str()).texture;
}

void GameViewWindow::ResizeIfNeed()
{
	if (!IsNearlyEqual(current_window_size, previous_window_size))
	{
		previous_window_size = current_window_size;
		renderer.ResizeViewport(previous_window_size.x, previous_window_size.y);
		texture = renderer.GetRenderTargetTexture(selected_render_target.c_str()).texture;
	}
}

void GameViewWindow::UpdateGuizmo()
{
	const auto input = InputManager::getInstance().player_input;

	if (input->WasJustPressed(EKeys::W))
	{
		current_gizmo_operation = ImGuizmo::TRANSLATE;
	}
	if (input->WasJustPressed(EKeys::E))
	{
		current_gizmo_operation = ImGuizmo::ROTATE;
	}
	if (input->WasJustPressed(EKeys::R))
	{
		current_gizmo_operation = ImGuizmo::SCALE;
	}
	if (input->WasJustPressed(EKeys::C))
	{
		SwitchOperationMode();
	}
}

void GameViewWindow::SendCameraData(float dt)
{
	view = editor_camera.GetViewMatrix();
	projection = GetProjectionMatrix();
	auto cameraPosition = editor_camera.GetPosition();

	DirectX::SimpleMath::Vector4 position(
		cameraPosition.x,
		cameraPosition.y,
		cameraPosition.z,
		0.0f);

	renderer.SetRenderData({ dt, view, projection, position });
}

DirectX::SimpleMath::Matrix GameViewWindow::GetProjectionMatrix()
{
	using namespace DirectX::SimpleMath;

	auto currentTexture = renderer.GetRenderTargetTexture(selected_render_target.c_str());
	float fow = DirectX::XMConvertToRadians(60.0f);
	float width = (float)currentTexture.width;
	float height = (float)currentTexture.height;
	float aspectRatio = width / height;
	float nearPlane = 0.01f;
	float farPlane = 10000.0f;

	return matrix::CreatePerspectiveFieldOfView(fow, aspectRatio, nearPlane, farPlane);;
}

void GameViewWindow::SwitchOperationMode()
{
	current_operation_mode = current_operation_mode == ImGuizmo::WORLD ? ImGuizmo::LOCAL : ImGuizmo::WORLD;
}

bool GameViewWindow::IsPlaying() const
{
	return is_playing;
}

void GameViewWindow::DrawMenu()
{
	with_MenuBar
	{
		if (ImGui::MenuItem("Play", "", is_playing, !is_playing))
		{
			EnterGameMode.Broadcast();
			is_playing = true;
			ImGui::GetIO().WantCaptureMouse = false;
		}
		if (ImGui::MenuItem("Stop", "", !is_playing, is_playing))
		{
			ExitGameMode.Broadcast();
			is_playing = false;
			ImGui::GetIO().WantCaptureMouse = true;
		}
	}
}

void GameViewWindow::DrawViewport() 
{
	with_Child("GameRender", {}, false, ImGuiWindowFlags_NoBackground)
	{
		if (texture == nullptr) 
		{
			texture = renderer.GetRenderTargetTexture(selected_render_target.c_str()).texture;
		}

		// This branch can contains child windows (it's necessary for HUD)
		// but can not capture inputs (it's necessary for gizmo)
		if (is_playing) 
		{
			auto size = ImGui::GetWindowSize();
			auto min = ImGui::GetWindowPos();
			auto max = ImGui::GetWindowPos();
			max.x += size.x;
			max.y += size.y;

			ImGui::GetWindowDrawList()->AddImage(texture, min, max);
		}
		// This branch can capture inputs (it's necessary for gizmo)
		// but can not contains child windows (it's bad for HUD)
		else 
		{
			ImGui::Image(texture, ImGui::GetWindowSize());
			if (ImGui::IsMouseClicked(0))
			{

				ImVec2 mouse_pos = ImGui::GetMousePos();
				ImVec2 window_pos = ImGui::GetWindowPos();
				ImVec2 real_pos;
				real_pos.x = mouse_pos.x - window_pos.x;
				real_pos.y = mouse_pos.y - window_pos.y;
				
				std::cout << this->renderer.GetObjectId((uint)real_pos.x, (uint)real_pos.y) << std::endl << std::flush;
			}

		}

		ViewportPresented.Broadcast();

		if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
		{
			is_camera_input_mode = GetCursorPos(&last_cursor_position);
			ShowCursor(!is_camera_input_mode);
		}

		if (is_camera_input_mode)
		{
			SetCursorPos(last_cursor_position.x, last_cursor_position.y);
		}

		if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
		{
			ShowCursor(true);
			is_camera_input_mode = false;
		}

		if (!is_playing && IsObjectWithTransformSelected())
		{
			DrawGizmo();
		}

		is_in_focus = ImGui::IsWindowFocused();
	}
}

void GameViewWindow::DrawRenderTargetSelector() 
{
	ImGuiWindowFlags window_flags = 
		ImGuiWindowFlags_NoDecoration | 
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_AlwaysAutoResize | 
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoFocusOnAppearing | 
		ImGuiWindowFlags_NoNav |
		ImGuiWindowFlags_NoMove;

	ImVec2 windowPosition = ImGui::GetWindowPos(), window_position_pivot = { 1.f, 0 };
	windowPosition.y += 50;
	windowPosition.x += ImGui::GetWindowSize().x - 10.f;

	ImGui::SetNextWindowPos(windowPosition, ImGuiCond_Always, window_position_pivot);
	ImGui::SetNextWindowBgAlpha(0.35f);

	with_Window("Overlay", &is_playing, window_flags)
	{
		const auto renderTargets = renderer.GetRenderTargetsList();

		if (ImGui::BeginCombo("###RenderTargetList", selected_render_target.c_str(), ImGuiComboFlags_NoArrowButton))
		{
			for (const auto renderTarget : renderTargets)
			{
				const bool isSelected = renderTarget == selected_render_target;

				if (ImGui::Selectable(renderTarget, isSelected))
				{
					selected_render_target = renderTarget;
					texture = renderer.GetRenderTargetTexture(selected_render_target.c_str()).texture;
				}

				if (isSelected) 
				{
					ImGui::SetItemDefaultFocus();
				}					
			}

			ImGui::EndCombo();
		}
	}
}

void GameViewWindow::DrawGizmo()
{
	ImGuizmo::SetOrthographic(false);
	ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

	const auto& transformType = transform_component->GetType();

	auto scaleProperty = transformType.GetProperty("LocalScale");
	auto rotationProperty = transformType.GetProperty("Rotation");
	auto positionProperty = transformType.GetProperty("Position");

	const auto scale = scaleProperty.GetValue(*transform_component).value().Unbox<DirectX::SimpleMath::Vector3>();
	const auto rotation = rotationProperty.GetValue(*transform_component).value().Unbox<DirectX::SimpleMath::Quaternion>();
	const auto position = positionProperty.GetValue(*transform_component).value().Unbox<DirectX::SimpleMath::Vector3>();

	auto model = Matrix::Identity;
	model *= Matrix::CreateScale(scale);
	model *= Matrix::CreateFromQuaternion(rotation);
	model *= Matrix::CreateTranslation(position);

	auto isManipulated = Manipulate(
		*view.m,
		*projection.m,
		current_gizmo_operation,
		current_operation_mode,
		*model.m);

	if (isManipulated && ImGuizmo::IsUsing())
	{
		DirectX::SimpleMath::Vector3 newScale;
		DirectX::SimpleMath::Quaternion newRotation;
		DirectX::SimpleMath::Vector3 newPosition;

		model.Decompose(newScale, newRotation, newPosition);

		if (current_gizmo_operation & ImGuizmo::TRANSLATE)
		{
			positionProperty.SetValue(*transform_component, &newPosition);
		}			
		else if (current_gizmo_operation & ImGuizmo::ROTATE)
		{
			rotationProperty.SetValue(*transform_component, &newRotation);
		}			
		else if (current_gizmo_operation & ImGuizmo::SCALE)
		{			
			scaleProperty.SetValue(*transform_component, &newScale);
		}					
	}
}

bool GameViewWindow::IsInCameraEditorInputMode() const
{
	return is_camera_input_mode && !is_playing;
}

bool GameViewWindow::IsObjectWithTransformSelected() const
{
	return game_object != nullptr && transform_component != nullptr;
}

bool GameViewWindow::IsNearlyEqual(ImVec2 lhs, ImVec2 rhs)
{
	return FMath::IsNearlyEqual(lhs.x, rhs.x)
		&& FMath::IsNearlyEqual(lhs.y, rhs.y);
}
