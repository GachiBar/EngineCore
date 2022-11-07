#pragma once


namespace engine
{
	class GameObject;
}

class EditorLayer;

class EditorCamera
{
public:
	EditorLayer* owner_layer;

	engine::GameObject* go;

	void Tick(float dt);

	float Yaw = 0;
	float Pitch = 0;
	float velocity_magnitude = 5.f;
};

