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

	mono::mono_object scene = m_Assembly.get_type("GameplayCore", "Scene").new_instance();
	mono::mono_object go1 = CreateGameObject(scene);
	mono::mono_object go2 = CreateGameObject(scene);

protected:
	//std::shared_ptr<MainWindow> mw;
};
