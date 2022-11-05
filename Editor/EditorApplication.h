#pragma once

#include "Application.h"

class EditorApplication final : public Application
{
public:
	EditorApplication();

	void OnSetup() override;

	void OnStart() override;

	std::shared_ptr<engine::Scene> scene;

	engine::GameObject* GetCamera() const;

	std::shared_ptr<engine::GameObject> test_go;

protected:
	std::shared_ptr<engine::GameObject> camera_go;
};
