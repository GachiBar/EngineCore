#include "Application.h"

Application::Application():engine_(new Engine())
{
}

int Application::Run()
{
	Setup();
	if (exitCode_)
		return exitCode_;

	engine_->Initialize();
	engine_->InitializeScene();

	Start();
	if (exitCode_)
		return exitCode_;

	//TODO add IsExiting
	while (!engine_->IsExiting())
		engine_->RunFrame();

	Stop();

	return exitCode_;
}
