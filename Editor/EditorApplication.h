#pragma once
#include "Application.h"
#include "MainWindow.h"
#include "Layer.h"

extern engine::CSharpDomain domain;
extern engine::CSharpAssembly assembly;
extern engine::CSharpObject CreateGameObject(engine::CSharpObject& scene);
extern engine::CSharpObject AddComponent(engine::CSharpObject& go, const std::string& name_space, const std::string& name);



class EditorApplication final : public Application
{
public:
	EditorApplication();

	void OnSetup() override;

	void OnStart() override;

	engine::CSharpObject scene = { domain, assembly, "GameplayCore", "Scene" };
	engine::CSharpObject go1 = CreateGameObject(scene);
	engine::CSharpObject go2 = CreateGameObject(scene);

protected:
	//std::shared_ptr<MainWindow> mw;
};
