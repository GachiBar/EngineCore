#include "SceneHierarchyWindow.h"
#include "imgui/imgui.h"
#include "../GameplaySystem/Scene.h"
#include "../GameplaySystem/Component.h"

#include <format>
#include <iostream>
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
    
    if (ImGui::CollapsingHeader("With transform", ImGuiTreeNodeFlags_DefaultOpen)) 
    {
        int id = 0;

        for (size_t i = 0; i < scene->Count(); ++i)
        {
            auto gameObject = (*scene)[i];
            auto transform = gameObject->GetComponent("GameplayCore.Components", "TransformComponent");

            if (transform == nullptr)
            {
                transfomlessGameObjects.push_back(gameObject);
                continue;
            }

            auto parentProperty = transform->GetProperty("Parent");

            if (parentProperty.GetValue().has_value())
            {
                continue;
            }

            BuildTree(*transform, id);
            id += 1;
        }
    } 

    if (ImGui::CollapsingHeader("Without transform", ImGuiTreeNodeFlags_DefaultOpen)) 
    {
        for (size_t i = 0; i < transfomlessGameObjects.size(); ++i)
        {
            auto gameObject = transfomlessGameObjects[i];
;
            ImGui::PushID(i);
            if (ImGui::Selectable(gameObject->Name().c_str(), IsSelected(gameObject)))
            {
                selected = gameObject;
                GameObjectSelected.Broadcast(gameObject);
            }
            ImGui::PopID();
        }
    }

    scene->Invalidate();

    ImGui::End();
}

void SceneHierarchyWindow::BuildTree(engine::Component& transform, int id)
{
    ImGui::PushID(id);

    auto gameObject = transform.GameObject();

    auto childrenCountProperty = transform.GetProperty("ChildrenCount");
    auto childrenCountObject = childrenCountProperty.GetValue().value();
    auto childrenCount = childrenCountObject.Unbox<size_t>();

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;

    if (childrenCount == 0)
    {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }
    if (IsSelected(gameObject))
    {
        flags |= ImGuiTreeNodeFlags_Selected;
    }

    bool isExpanded = ImGui::TreeNodeEx(gameObject->Name().c_str(), flags);

    if (isExpanded)
    {
        if (ImGui::IsItemClicked())
        {
            selected = gameObject;
            GameObjectSelected.Broadcast(gameObject);
        }        

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
        
    DrawPopup(gameObject);

    if (!isExpanded && ImGui::IsItemClicked())
    {
        selected = gameObject;
        GameObjectSelected.Broadcast(gameObject);
    }

    ImGui::PopID();
}

void SceneHierarchyWindow::DrawPopup(std::shared_ptr<engine::GameObject> gameObject)
{
    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::Selectable("Delete"))
        {
            scene->DeleteGameObject(gameObject);
        }

        ImGui::Separator();

        if (ImGui::Selectable("Create empty"))
        {
            auto parentTransform = gameObject->GetComponent("GameplayCore.Components", "TransformComponent");
            
            auto child = scene->CreateGameObject();
            scene->Invalidate();
            
            auto childTransform = child->AddComponent("GameplayCore.Components", "TransformComponent");
            child->Invalidate();

            auto parentProperty = childTransform->GetProperty("Parent");
            parentProperty.SetValue(*parentTransform);
        }

        ImGui::EndPopup();
    }
}

bool SceneHierarchyWindow::IsSelected(std::shared_ptr<engine::GameObject> gameObject) 
{
    return selected != nullptr && *gameObject == *selected;
}
