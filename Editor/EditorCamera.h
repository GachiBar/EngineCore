#pragma once

#include "SimpleMath.h"

struct RenderDevice;
class EditorLayer;

class EditorCamera final
{
public:
	DirectX::SimpleMath::Matrix View;
	DirectX::SimpleMath::Matrix Proj;

	float mouse_sensitivity = 1.f;

	DirectX::SimpleMath::Matrix GetCameraMatrix() const;

	DirectX::SimpleMath::Vector3 CameraPos;

	virtual void UpdateProjectionMatrix();

	EditorLayer* owner_layer;

	void Tick(float dt);

	float Yaw = 0;
	float Pitch = 0;
	float velocity_magnitude = 5.f;
};

