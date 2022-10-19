#include "SceneHierarchyWindow.h"
#include "../monowrapper/monopp/mono_method.h"
#include "../monowrapper/monopp/mono_method_invoker.h"
#include "../monowrapper/monopp/mono_property_invoker.h"
#include "../Editor/InputSystem/InputManager.h"
#include "imgui/imgui_sugar.hpp"
#include "format"

void SceneHierarchyWindow::draw_imgui(mono::mono_object& scene)
{
    auto go_arr = GetGameObjects(scene);
    with_Window("Scene hierarchy")
    {
	    with_ListBox("Scene hierarchy")
	    {
            for (int i = 0; i < go_arr.size(); ++i)
            {
                if (ImGui::Selectable(std::format("go_{}", i).c_str(), false))
                {
                    OnSelectGameObjectInHierarchy.Execute(go_arr[i]);
                }
            }
	    }
    }
}

std::vector<std::shared_ptr<mono::mono_object>> SceneHierarchyWindow::GetGameObjects(mono::mono_object& scene) const
{
    std::vector<std::shared_ptr<mono::mono_object>> res;

    mono::mono_property game_objects_count_property = scene.get_type().get_property("Count");
    mono::mono_property_invoker game_objects_count_property_invoker(game_objects_count_property);
    mono::mono_object game_objects_count_object(game_objects_count_property_invoker.get_value(scene));
    int game_objects_count = game_objects_count_object.unbox<int>();

    res.reserve(game_objects_count);

    // Iterate over all game objects.
    for (int i = 0; i < game_objects_count; ++i) {
        mono::mono_method get_game_object_method = scene.get_type().get_method("get_Item");
        mono::mono_method_invoker get_game_object_method_invoker(get_game_object_method);

        void* args[1];
        args[0] = &i;

        mono::mono_object go(get_game_object_method_invoker.invoke(scene, args));
        res.push_back(std::make_shared< mono::mono_object>(mono::mono_object(get_game_object_method_invoker.invoke(scene, args))));
    }

    return res;
}
