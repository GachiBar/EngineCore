#include "EditorCamera.h"
#include "InputManager.h"
#include "libs/MathUtility.h"

DirectX::SimpleMath::Vector3 EditorCamera::GetPosition()
{
	return position;
}

DirectX::SimpleMath::Matrix EditorCamera::GetViewMatrix()
{
	return view;
}

void EditorCamera::Tick(float dt)
{
	using namespace DirectX::SimpleMath;

	UpdateYawPitch(dt);
	UpdateSpeed();
	UpdatePosition(dt);

	view = Matrix::CreateLookAt(position, position + rotation.Forward(), rotation.Up());
}

void EditorCamera::UpdateYawPitch(float dt)
{
	using namespace DirectX::SimpleMath;

	auto input = InputManager::getInstance().player_input.get();
	const float dx = input->GetKeyValue(EKeys::MouseX);
	const float dy = input->GetKeyValue(EKeys::MouseY);
	const float angularSpeed = 0.1f;
	const float weight = 0.003f;

	yaw -= dx * weight * kMouseSensitivity;
	pitch -= dy * weight * kMouseSensitivity;

	if (input->IsPressed(EKeys::Left)) 
	{
		yaw += angularSpeed * dt;
	}		
	if (input->IsPressed(EKeys::Right))
	{
		yaw -= angularSpeed * dt;
	}		
	if (input->IsPressed(EKeys::Up))
	{
		pitch += angularSpeed * dt;
	}		
	if (input->IsPressed(EKeys::Down))
	{
		pitch -= angularSpeed * dt;
	}

	rotation = Matrix::CreateFromYawPitchRoll(yaw, pitch, 0);
}

void EditorCamera::UpdatePosition(float dt)
{
	auto axis = GetAxis();

	if (axis.Length() != 0)
	{
		axis.Normalize();
	}
	
	auto direction =
		rotation.Forward() * axis.x +
		rotation.Right() * axis.z +
		DirectX::SimpleMath::Vector3::Up * axis.y;

	if (direction.Length() != 0)
	{
		direction.Normalize();
	}

	auto velocity = direction * speed;
	position += velocity * dt;
}

void EditorCamera::UpdateSpeed()
{
	auto input = InputManager::getInstance().player_input.get();
	const float wheelDelta = input->GetKeyValue(EKeys::MouseWheelAxis);

	if (wheelDelta > 0.0f)
	{
		speed += 1;
	}
	else if (wheelDelta < 0.0f)
	{
		speed -= 1;
	}

	speed = FMath::Clamp(speed, 1.f, 10.f);
}

DirectX::SimpleMath::Vector3 EditorCamera::GetAxis()
{
	using namespace DirectX::SimpleMath;

	auto input = InputManager::getInstance().player_input.get();
	Vector3 axis = Vector3::Zero;

	if (input->IsPressed(EKeys::W)) 
	{
		axis += Vector3::UnitX;
	}
	if (input->IsPressed(EKeys::S)) 
	{
		axis -= Vector3::UnitX;
	}
	if (input->IsPressed(EKeys::A))
	{
		axis -= Vector3::UnitZ;
	}
	if (input->IsPressed(EKeys::D))
	{
		axis += Vector3::UnitZ;
	}
	if (input->IsPressed(EKeys::SpaceBar))
	{
		axis += Vector3::UnitY;
	}
	if (input->IsPressed(EKeys::C))
	{
		axis -= Vector3::UnitY;
	}
	
	return axis;
}
