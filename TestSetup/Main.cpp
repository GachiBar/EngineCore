#include <iostream>

#include "../GameplaySystem/Engine.h"
#include "../monowrapper/monopp/mono_jit.h"

//TODO Move application header to another project
#include "../Editor/Application.h"
#include "../Editor/EditorApplication.h"

const char kMonoLibPath[] = "..\\vendor\\mono\\lib\\4.5";
const char kDllPath[] = "GameplayCore.dll";

class StandaloneGameTestApplication final : public Application
{
public:
	StandaloneGameTestApplication(const char* dll_path) 
		: Application(dll_path)
	{}

	mono::mono_object scene{ m_Assembly.get_type("GameplayCore", "Scene").new_instance() };
	mono::mono_object go1{ CreateGameObject(scene) };
	mono::mono_object go2{ CreateGameObject(scene) };

	void OnSetup() override;
};


void StandaloneGameTestApplication::OnSetup()
{
	AddComponent(m_Assembly, go1, "GameplayCore.Components", "TestUpdateComponent");
	AddComponent(m_Assembly, go2, "GameplayCore.Components", "TestFixedUpdateComponent");
	AddComponent(m_Assembly, go1, "GameplayCore.Components", "RenderComponent");
	AddComponent(m_Assembly, go1, "GameplayCore.Components", "TransformComponent");

	engine_->SetScene(&scene);
}
#include "../Editor/Delegates.h"

int main() {
	if (!mono::init_with_mono_assembly_path(kMonoLibPath, "KtripRuntime")) {
		return 1;
	}		

	//StandaloneGameTestApplication app(kDllPath);
	EditorApplication app(kDllPath);
	return app.Run();
}
