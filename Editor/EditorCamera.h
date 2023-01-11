#pragma once

#include "SimpleMath.h"

class EditorCamera final
{
public:
	const float kMouseSensitivity = 1.0f;
	
	DirectX::SimpleMath::Vector3 GetPosition();
	DirectX::SimpleMath::Matrix GetViewMatrix();

	void Tick(float dt);
private:
	DirectX::SimpleMath::Vector3 position;
	DirectX::SimpleMath::Matrix rotation;
	DirectX::SimpleMath::Matrix view;

	float yaw = 0;
	float pitch = 0;
	float speed = 5.0f;

	void UpdateYawPitch(float dt);	
	void UpdatePosition(float dt);
	void UpdateSpeed();
	DirectX::SimpleMath::Vector3 GetAxis();
};

