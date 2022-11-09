#include "SceneHierarchyWindow.h"
#include "imgui/imgui.h"
#include "../GameplaySystem/Scene.h"
#include "../GameplaySystem/Component.h"

#include <format>
#include <imgui/imgui_internal.h>

std::shared_ptr<engine::Scene> SceneHierarchyWindow::GetScene()
{
    return scene;
}

void SceneHierarchyWindow::SetScene(std::shared_ptr<engine::Scene> scene)
{
    this->scene = scene;
}

SceneHierarchyWindow::SceneHierarchyWindow(const mono::mono_assembly& assembly)
    : assembly(assembly)
{}

void SceneHierarchyWindow::draw_imgui()
{
    ImGui::Begin("Scene Hierarchy");
    std::vector<std::shared_ptr<engine::GameObject>> transfomlessGameObjects;

    for (size_t i = 0; i < scene->Count(); ++i)
    {
        auto gameObject = (*scene)[i];
        transfomlessGameObjects.push_back(gameObject);
    }

    //if (ImGui::CollapsingHeader("With transform", ImGuiTreeNodeFlags_DefaultOpen)) 
    //{
    //    for (size_t i = 0; i < scene->Count(); ++i)
    //    {
    //        auto gameObject = (*scene)[i];
    //        auto transform = gameObject->GetComponent("GameplayCore.Components", "TransformComponent");

    //        if (transform == nullptr)
    //        {
    //            transfomlessGameObjects.push_back(gameObject);
    //            continue;
    //        }

    //        auto parentProperty = transform->GetProperty("Parent");

    //        if (parentProperty.GetValue().has_value())
    //        {
    //            continue;
    //        }

    //        BuildTree(*transform, 0);
    //    }
    //} 

    if (ImGui::CollapsingHeader("Without transform", ImGuiTreeNodeFlags_DefaultOpen)) 
    {
        for (size_t i = 0; i < transfomlessGameObjects.size(); ++i)
        {
            auto gameObject = transfomlessGameObjects[i];
;
            ImGui::PushID(i);
            if (ImGui::Selectable(gameObject->Name().c_str(), false))
            {
                GameObjectSelected.Broadcast(gameObject);
            }
            ImGui::PopID();
        }
    }

    ImGui::End();
}

void SceneHierarchyWindow::BuildTree(engine::Component& transform, int id)
{
    ImGui::PushID(id);
    auto owner = transform.GameObject();
    bool isSelected = false;

    if (SelectableTreeNode(owner->Name().c_str(), &isSelected)) 
    {
        if (isSelected) 
        {
            GameObjectSelected.Broadcast(owner);
        }

        auto childrenCountProperty = transform.GetProperty("ChildrenCount");
        auto childrenCountObject = childrenCountProperty.GetValue().value();
        auto childrenCount = childrenCountObject.Unbox<size_t>();

        auto getChildMethod = transform.GetMethod("GetChild", 1);

        for (size_t i = 0; i < childrenCount; ++i)
        {
            void* params[1];
            params[0] = &i;

            auto childObject = getChildMethod.Invoke(params).value();
            engine::Component childTransform(assembly, childObject.GetInternal());
            BuildTree(childTransform, id + 1);  
        }
                
        ImGui::TreePop();
    }

    ImGui::PopID();
}

bool SceneHierarchyWindow::SelectableTreeNode(const char* label, bool* isSelected)
{    
    bool isExpanded = ImGui::TreeNode(label, "");

    //ImGui::PushID(id);
    //bool isExpanded = ImGui::TreeNodeBehavior(id, 0, "", NULL);
    //bool isExpanded = ImGui::TreeNode("");
    //ImGui::PopID();

    ImGui::SameLine();    
    *isSelected = ImGui::Selectable(label, false);   
    return isExpanded;
}

