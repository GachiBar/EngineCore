#pragma once

#include "Application.h"

class EditorApplication final : public Application
{
public:
	EditorApplication();

	void OnSetup() override;

	void OnStart() override;

	std::shared_ptr<engine::Scene> scene;

protected:
	//std::shared_ptr<MainWindow> mw;
};
