#pragma once

#include "Application.h"
#include "MainWindow.h"
#include "Layer.h"

class EditorApplication final : public Application
{
public:
	EditorApplication(const char* dll_path);

	void OnSetup() override;

	void OnStart() override;

	Scene scene;
	GameObject* game_object_1 = nullptr;
	GameObject* game_object_2 = nullptr;

protected:
	//std::shared_ptr<MainWindow> mw;
};
