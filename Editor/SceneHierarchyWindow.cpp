#include "SceneHierarchyWindow.h"
#include "../monowrapper/monopp/mono_method.h"
#include "../monowrapper/monopp/mono_method_invoker.h"
#include "../monowrapper/monopp/mono_property_invoker.h"
#include "../Editor/InputSystem/InputManager.h"
#include "imgui/imgui.h"
#include "format"

void SceneHierarchyWindow::draw_imgui(const engine::Scene& scene)
{
    ImGui::Begin("Scene Hierarchy");

    if (ImGui::BeginListBox("Scene Hierarchy"))
    {
        for (size_t i = 0; i < scene.Count(); ++i)
        {
            auto gameObject = scene[i];

            if(ImGui::Selectable(gameObject->Name().c_str(), false))
            {
                auto selectedGameObject = scene[i];
                OnSelectGameObjectInHierarchy.Execute(selectedGameObject);
            }
        }
        ImGui::EndListBox();
    }

    ImGui::End();
}