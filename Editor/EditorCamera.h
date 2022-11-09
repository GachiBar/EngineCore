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

	void UpdateEditorViewProjectionMatrix(float dt) const;
	void UpdateProjectionMatrix();
	EditorLayer* owner_layer;

	void Tick(float dt);
private:

	DirectX::SimpleMath::Vector3 CameraPos;

	float Yaw = 0;
	float Pitch = 0;
	float velocity_magnitude = 5.f;
};

