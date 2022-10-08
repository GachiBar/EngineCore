#pragma once
#include "Application.h"
#include "MainWindow.h"

class EditorApplication : public Application
{
public:
	EditorApplication();

	void Setup() override;

protected:
	std::shared_ptr<MainWindow> mw;
};
