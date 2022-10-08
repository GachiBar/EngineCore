#include "Application.h"

Application::Application():engine_(new Engine()),exit_code_(0)
{
}

int Application::Run()
{
	Setup();
	if (exit_code_)
		return exit_code_;

	engine_->Initialize();
	engine_->InitializeScene();

	Start();
	if (exit_code_)
		return exit_code_;

	while (!engine_->IsExiting())
		engine_->RunFrame();

	Stop();

	return exit_code_;
}
