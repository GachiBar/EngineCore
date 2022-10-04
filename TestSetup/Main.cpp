#include "../GameplaySystem/Game.h"
#include "../GameplaySystem/CSharpDomain.h"
#include "../GameplaySystem/CSharpAssembly.h"
#include "../GameplaySystem/CSharpObject.h"

engine::CSharpDomain domain("..\\vendor\\mono\\lib", "KtripRuntime", "KtripDomain");
engine::CSharpAssembly assembly("GameplayCore.dll");

engine::CSharpObject CreateGameObject(engine::CSharpObject& scene) {
	MonoObject* go_mono_object = scene.CallMethod("CreateGameObject");
	engine::CSharpObject go(domain, assembly, go_mono_object);
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
	engine::CSharpObject component(domain, assembly, component_mono_object);
	return component;
}

int main() {
	engine::Game game;

	engine::CSharpObject scene(domain, assembly, "GameplayCore", "Scene");	
	engine::CSharpObject go1 = CreateGameObject(scene);
	engine::CSharpObject go2 = CreateGameObject(scene);

	AddComponent(go1, "GameplayCore", "TestUpdateComponent");
	AddComponent(go2, "GameplayCore", "TestFixedUpdateComponent");

	game.SetScene(&scene);
	game.Run();

	return 0;
}