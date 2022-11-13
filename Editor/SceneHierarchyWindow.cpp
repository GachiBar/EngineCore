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
    , tree_level_id(0)
{}

void SceneHierarchyWindow::draw_imgui()
{
    ImGui::Begin("Scene Hierarchy");
    std::vector<std::shared_ptr<engine::GameObject>> transfomlessGameObjects;
    tree_level_id = 0;

    if (ImGui::CollapsingHeader("With transform", ImGuiTreeNodeFlags_DefaultOpen)) 
    {
        DrawWithTransformHeaderPopup();
        ProcessWithTransformHeaderDragAndDrop();

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

            DrawHierarchy(*transform);
            tree_level_id += 1;
        }
    }        
    else 
    {
        DrawWithTransformHeaderPopup();
    }    

    if (ImGui::CollapsingHeader("Without transform", ImGuiTreeNodeFlags_DefaultOpen)) 
    {
        for (size_t i = 0; i < transfomlessGameObjects.size(); ++i)
        {
            auto gameObject = transfomlessGameObjects[i];
            ImGui::PushID(tree_level_id);

            if (ImGui::Selectable(gameObject->Name().c_str(), IsSelected(gameObject)))
            {
                selected = gameObject;
                GameObjectSelected.Broadcast(gameObject);
            }

            tree_level_id += 1;
            ImGui::PopID();
        }
    }

    ProcessHierarchyDragAndDrop();
    scene->Invalidate();
    ImGui::End();
}

void SceneHierarchyWindow::DrawWithTransformHeaderPopup()
{
    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::Selectable("Create empty"))
        {
            auto child = scene->CreateGameObject();
            scene->Invalidate();

            child->AddComponent("GameplayCore.Components", "TransformComponent");
            child->Invalidate();
        }

        ImGui::EndPopup();
    }
}

void SceneHierarchyWindow::DrawHierarchy(engine::Component& transform)
{
    ImGui::PushID(tree_level_id);
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
    DrawSelected(gameObject);
    SetupDragSource(gameObject);
    SetupDropSource(gameObject);
    DrawPopup(gameObject);

    if (isExpanded)
    {
        auto getChildMethod = transform.GetMethod("GetChild", 1);

        for (size_t i = 0; i < childrenCount; ++i)
        {
            void* params[1];
            params[0] = &i;

            auto childObject = getChildMethod.Invoke(params).value();
            engine::Component childTransform(assembly, childObject.GetInternal());            
            DrawHierarchy(childTransform);
            tree_level_id += 1;
        }
                
        ImGui::TreePop();
    } 
    
    ImGui::PopID();
}

void SceneHierarchyWindow::DrawSelected(std::shared_ptr<engine::GameObject> gameObject)
{
    if (ImGui::IsItemClicked())
    {
        selected = gameObject;
        GameObjectSelected.Broadcast(gameObject);
    }
}

void SceneHierarchyWindow::SetupDragSource(std::shared_ptr<engine::GameObject> gameObject)
{
    if (ImGui::BeginDragDropSource())
    {
        ImGui::SetDragDropPayload("_TREENODE", &gameObject, sizeof(std::shared_ptr<engine::GameObject>));
        drag_and_drop_request.source = gameObject;
        ImGui::Text(gameObject->Name().c_str());
        ImGui::EndDragDropSource();
    }
}

void SceneHierarchyWindow::SetupDropSource(std::shared_ptr<engine::GameObject> gameObject)
{
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_TREENODE"))
        {
            auto dragged = (std::shared_ptr<engine::GameObject>*)payload->Data;
            drag_and_drop_request.target = gameObject;
        }

        ImGui::EndDragDropTarget();
    }
}

void SceneHierarchyWindow::DrawPopup(std::shared_ptr<engine::GameObject> gameObject)
{
    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::Selectable("Delete"))
        {
            auto transform = gameObject->GetComponent("GameplayCore.Components", "TransformComponent");
            DeleteHierarchy(*transform);
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

void SceneHierarchyWindow::DeleteHierarchy(engine::Component& transform)
{
    auto gameObject = transform.GameObject();
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
        DeleteHierarchy(childTransform);
    }

    scene->DeleteGameObject(gameObject);
}

void SceneHierarchyWindow::ProcessWithTransformHeaderDragAndDrop()
{
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_TREENODE"))
        {
            auto dragged = (std::shared_ptr<engine::GameObject>*)payload->Data;
            auto source = *dragged;
            auto sourceTransform = source->GetComponent("GameplayCore.Components", "TransformComponent");
            auto parentProperty = sourceTransform->GetProperty("Parent");
            parentProperty.SetValue(nullptr);
        }

        ImGui::EndDragDropTarget();
    }
}

void SceneHierarchyWindow::ProcessHierarchyDragAndDrop()
{
    if (drag_and_drop_request.IsRequested())
    {        
        auto targetTransform = drag_and_drop_request.target->GetComponent("GameplayCore.Components", "TransformComponent");                 
        auto sourceTransform = drag_and_drop_request.source->GetComponent("GameplayCore.Components", "TransformComponent");
        auto parentProperty = sourceTransform->GetProperty("Parent");
        parentProperty.SetValue(*targetTransform);
        drag_and_drop_request.Reset();
    }
}

bool SceneHierarchyWindow::DragAndDropRequest::IsRequested()
{
    return source != nullptr && target != nullptr;
}

void SceneHierarchyWindow::DragAndDropRequest::Reset()
{
    source = nullptr;
    target = nullptr;
}
