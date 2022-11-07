#include "EditorCamera.h"

#include "InputManager.h"
#include "../GameplaySystem/Engine.h"
#include "../GameplaySystem/Component.h"
#include "EditorLayer.h"

//TODO get this from input settings
static float mouse_sensitivity = 1.f;

void EditorCamera::Tick(float dt)
{
	using namespace DirectX::SimpleMath;

	PlayerInput* const player_input = InputManager::getInstance().player_input.get();

	auto value = static_cast<float*>(go->GetComponent("GameplayCore.Components", "TransformComponent")->GetProperty("ModelMatrix").GetValue().value().unbox());
	Matrix t{ DirectX::XMFLOAT4X4(value) };
	
	Yaw -= player_input->GetKeyValue(EKeys::MouseX) * 0.003f * mouse_sensitivity;
	Pitch -= player_input->GetKeyValue(EKeys::MouseY) * 0.003f * mouse_sensitivity;

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


	auto velDir = t.Forward() * velDirection.x + Vector3::Up * velDirection.y + t.Right() * velDirection.z;

	if (velDir.Length() != 0)
		velDir.Normalize();

	auto LocationOffset = velDir * velocity_magnitude * dt;

	auto OffsetMatrix = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(Pitch * 180.f, Yaw * 180.f, 0.f) * Matrix::CreateTranslation(LocationOffset);
	auto test = OffsetMatrix.Translation();
	auto result = t * DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(Pitch * 180.f, Yaw * 180.f, 0.f) * Matrix::CreateTranslation(LocationOffset);
	//go->GetComponent("GameplayCore.Components", "TransformComponent")->GetProperty("ModelMatrix").SetValue(result.m);

	auto transform = go->AddComponent("GameplayCore.Components", "TransformComponent");

	auto property = transform->GetProperty("LocalPosition");

	auto resultLoc = result.Translation();
	property.SetValue(&resultLoc);
	auto property2 = transform->GetProperty("LocalRotation");
	auto resultRot = Quaternion::CreateFromRotationMatrix(result);
	property2.SetValue(&resultRot);
}
