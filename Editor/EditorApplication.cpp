#include "EditorApplication.h"
#include "EditorLayer.h"
#include "../monowrapper/monopp/mono_property.h"
#include "../monowrapper/monopp/mono_property_invoker.h"

EditorApplication::EditorApplication(const char* dll_path):Application(dll_path) //,mw(new MainWindow)
{
}

void EditorApplication::Setup()
{
	AddComponent(m_Assembly, go1, "GameplayCore.Components", "TestUpdateComponent");
	AddComponent(m_Assembly, go2, "GameplayCore.Components", "TestFixedUpdateComponent");
	AddComponent(m_Assembly, go1, "GameplayCore.Components", "MeshRenderComponent");
	AddComponent(m_Assembly, go1, "GameplayCore.Components", "TransformComponent");

	engine_->SetScene(&scene);

	auto test = scene.get_type().get_methods();
	auto test2 = scene.get_type().get_properties();

	mono::mono_property game_objects_count_property = scene.get_type().get_property("Count");
	mono::mono_property_invoker game_objects_count_property_invoker(game_objects_count_property);
	mono::mono_object game_objects_count_object(game_objects_count_property_invoker.get_value(scene));
	int game_objects_count = game_objects_count_object.unbox<int>();

	// Iterate over all game objects.
	for (int i = 0; i < game_objects_count; ++i) {
		mono::mono_method get_game_object_method = scene.get_type().get_method("get_Item");
		mono::mono_method_invoker get_game_object_method_invoker(get_game_object_method);

		void* args[1];
		args[0] = &i;

		mono::mono_object go(get_game_object_method_invoker.invoke(scene, args));

		mono::mono_property components_count_property = go.get_type().get_property("Count");
		mono::mono_property_invoker components_count_property_invoker(components_count_property);
		mono::mono_object components_count_object(components_count_property_invoker.get_value(go));
		int components_count = components_count_object.unbox<int>();

		// Iterate over all game object's components.
		for (int j = 0; j < components_count; ++j) {
			mono::mono_method get_component_method = go.get_type().get_method("get_Item");
			mono::mono_method_invoker get_component_method_invoker(get_component_method);

			void* args[1];
			args[0] = &j;

			mono::mono_object component(get_component_method_invoker.invoke(go, args));
			// TODO: iterate over components properties (component.get_type().get_properties())
		}
	}

	const auto editor_layer = new EditorLayer(&m_LayerStack);
	PushLayer(editor_layer);
}
