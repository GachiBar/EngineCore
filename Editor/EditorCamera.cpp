#include "EditorCamera.h"

#include "InputManager.h"
#include "../GameplaySystem/Engine.h"
#include "../GameplaySystem/Component.h"
#include "EditorLayer.h"
#include "RenderEngine.h"

//TODO get this from input settings
static float mouse_sensitivity = 1.f;

DirectX::SimpleMath::Matrix EditorCamera::GetCameraMatrix() const
{
	return View * Proj;
}

void EditorCamera::UpdateProjectionMatrix()
{
	auto out_texture = owner_layer->GetApp()->GetEngine()->GetRenderer().GetRenderTargetTexture("outTexture");
	Proj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(DirectX::XMConvertToRadians(60.f), (float)out_texture.width/ (float)out_texture.height, 0.1f, 100000.f);
}

void EditorCamera::Tick(float dt)
{
	using namespace DirectX::SimpleMath;

	PlayerInput* const player_input = InputManager::getInstance().player_input.get();
	
	Yaw -= player_input->GetKeyValue(EKeys::MouseX) * 0.003f * mouse_sensitivity;
	Pitch -= player_input->GetKeyValue(EKeys::MouseY) * 0.003f * mouse_sensitivity;
	auto rotMat = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(Yaw, Pitch, 0);

	const float WheelAxisValue = player_input->GetAxisValue("Editor_WheelAxis");
	if(WheelAxisValue >0.f)
	{
		velocity_magnitude += 1;
	}
	else if(WheelAxisValue < 0.f)
	{
		velocity_magnitude -= 1;
	}

	auto velDirection = Vector3::Zero;

	if (player_input->IsPressed(EKeys::W)) velDirection += Vector3(1.f, 0.f, 0.f);
	if (player_input->IsPressed(EKeys::S)) velDirection += Vector3(-1.f, 0.f, 0.f);
	if (player_input->IsPressed(EKeys::A)) velDirection += Vector3(0.f, 0.f, -1.f);
	if (player_input->IsPressed(EKeys::D)) velDirection += Vector3(0.f, 0.f, 1.f);

	if (player_input->IsPressed(EKeys::SpaceBar)) velDirection += Vector3(0.f, 1.f, 0.f);
	if (player_input->IsPressed(EKeys::C)) velDirection += Vector3(0.f, -1.f, 0.f);

	if (player_input->IsPressed(EKeys::NumPadFour)) Yaw += 0.1f * dt;
	if (player_input->IsPressed(EKeys::NumPadSix)) Yaw -= 0.1f * dt;
	if (player_input->IsPressed(EKeys::NumPadEight)) Pitch += 0.1f * dt;
	if (player_input->IsPressed(EKeys::NumPadTwo)) Pitch -= 0.1f * dt;

	if (velDirection.Length() != 0)
		velDirection.Normalize();

	auto velDir = rotMat.Forward() * velDirection.x + Vector3::Up * velDirection.y + rotMat.Right() * velDirection.z;

	if (velDir.Length() != 0)
		velDir.Normalize();

	CameraPos += velDir * velocity_magnitude * dt;

	View = DirectX::SimpleMath::Matrix::CreateLookAt(CameraPos, CameraPos + rotMat.Forward(), rotMat.Up());
	UpdateProjectionMatrix();

	owner_layer->GetApp()->GetEngine()->GetRenderer().SetRenderData({ dt,View,Proj });
}