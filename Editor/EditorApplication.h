#pragma once

#include "Application.h"
#include "MainWindow.h"
#include "Layer.h"

class EditorApplication final : public Application
{
public:
	EditorApplication();
	~EditorApplication();

	void OnSetup() override;

	void OnStart() override;

	Scene* scene;
	std::shared_ptr<GameObject> game_object_1;
	std::shared_ptr<GameObject> game_object_2;

protected:
	//std::shared_ptr<MainWindow> mw;
};
