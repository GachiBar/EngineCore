#include <iostream>

#include "../GameplaySystem/Game.h"
#include "../GameplaySystem/CSharpDomain.h"
#include "../GameplaySystem/CSharpAssembly.h"
#include "../GameplaySystem/CSharpObject.h"

engine::CSharpDomain domain("..\\vendor\\mono\\lib\\4.5", "KtripRuntime", "KtripDomain");
engine::CSharpAssembly assembly("GameplayCore.dll");

engine::CSharpObject CreateGameObject(engine::CSharpObject& scene) {
	MonoObject* go_mono_object = scene.CallMethod("CreateGameObject");
	engine::CSharpObject go(domain, assembly, go_mono_object);
	scene.CallMethod("Invalidate");
	return go;
}

engine::CSharpObject AddComponent(engine::CSharpObject& go, const std::string& name_space, const std::string& name) {
	MonoImage* image = mono_assembly_get_image(assembly.GetMonoAssembly());
	MonoClass* clazz = mono_class_from_name(image, name_space.c_str(), name.c_str());
	std::string type_name = name_space + "." + name;
	MonoType* mono_type = mono_reflection_type_from_name(type_name.data(), image);
	MonoReflectionType* mono_reflection_type = mono_type_get_object(domain.GetAppMonoDomain(), mono_type);

	void* params[1];
	params[0] = mono_reflection_type;

	MonoObject* component_mono_object = go.CallMethod("AddComponent", params, 1);
	go.CallMethod("Invalidate");
	engine::CSharpObject component(domain, assembly, component_mono_object);
	return component;
}

//TODO Move application header to another project
#include "../Editor/Application.h"
#include "../Editor/EditorApplication.h"

class StandaloneGameTestApplication final : public Application
{
public:
	engine::CSharpObject scene = { domain, assembly, "GameplayCore", "Scene" };
	engine::CSharpObject go1 = CreateGameObject(scene);
	engine::CSharpObject go2 = CreateGameObject(scene);

	void Setup() override;
};


void StandaloneGameTestApplication::Setup()
{
	AddComponent(go1, "GameplayCore.Components", "TestUpdateComponent");
	AddComponent(go2, "GameplayCore.Components", "TestFixedUpdateComponent");
	AddComponent(go1, "GameplayCore.Components", "RenderComponent");

	engine_->SetScene(&scene);
}
#include "../Editor/Delegates.h"

int main() {

	//std::cout << "Lambda delegate return value: " << del.Execute(3,5) << std::endl;

	//StandaloneGameTestApplication app;
	EditorApplication app;
	return app.Run();
}
