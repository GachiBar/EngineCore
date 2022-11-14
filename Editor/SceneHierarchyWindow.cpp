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
    std::vector<std::shared_ptr<engine::GameObject>> roots;
    std::vector<std::shared_ptr<engine::GameObject>> transformless;
    SortGameObjects(roots, transformless);
    tree_level_id = 0;

    DrawWithTransform(roots);
    DrawWithoutTransform(transformless);
    ProcessDragAndDrop();

    scene->Invalidate();
    ImGui::End();
}

void SceneHierarchyWindow::DrawWithTransform(const std::vector<std::shared_ptr<engine::GameObject>>& roots) 
{
    if (ImGui::CollapsingHeader("With transform", ImGuiTreeNodeFlags_DefaultOpen))
    {
        DrawWithTransformHeaderPopup();
        SetupDropTarget<WithTransformDragAndDropHandler>();

        for (auto root : roots) 
        {
            auto transform = root->GetComponent("GameplayCore.Components", "TransformComponent");
            DrawHierarchy(*transform);
            tree_level_id += 1;
        }
    }
    else
    {
        DrawWithTransformHeaderPopup();
        SetupDropTarget<WithTransformDragAndDropHandler>();
    }
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

void SceneHierarchyWindow::DrawWithoutTransformHeaderPopup()
{
    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::Selectable("Create empty"))
        {
            auto child = scene->CreateGameObject();
            scene->Invalidate();
        }

        ImGui::EndPopup();
    }
}

void SceneHierarchyWindow::DrawHierarchyPopup(std::shared_ptr<engine::GameObject> gameObject)
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
    DrawHierarchyPopup(gameObject);

    SetupDragSource(gameObject);
    SetupDropTarget<HierarchyDragAndDropHandler>(gameObject);
    
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

void SceneHierarchyWindow::DrawWithoutTransform(
    const std::vector<std::shared_ptr<engine::GameObject>>& transformless) 
{
    if (ImGui::CollapsingHeader("Without transform", ImGuiTreeNodeFlags_DefaultOpen))
    {
        DrawWithoutTransformHeaderPopup();
        SetupDropTarget<WithoutTransformDragAndDropHandler>();

        for (size_t i = 0; i < transformless.size(); ++i)
        {
            ImGui::PushID(tree_level_id);
            auto gameObject = transformless[i];

            if (ImGui::Selectable(gameObject->Name().c_str(), IsSelected(gameObject)))
            {
                selected = gameObject;
                GameObjectSelected.Broadcast(gameObject);
            }

            SetupDragSource(gameObject);            

            tree_level_id += 1;
            ImGui::PopID();
        }
    }
    else
    {
        DrawWithoutTransformHeaderPopup();
        SetupDropTarget<WithoutTransformDragAndDropHandler>();
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

void SceneHierarchyWindow::SetupDragSource(std::shared_ptr<engine::GameObject> gameObject)
{
    if (ImGui::BeginDragDropSource())
    {
        ImGui::SetDragDropPayload("_TREENODE", nullptr, 0);
        drag_and_drop_request.Source = gameObject;
        ImGui::Text(gameObject->Name().c_str());
        ImGui::EndDragDropSource();
    }
}

void SceneHierarchyWindow::ProcessDragAndDrop()
{
    if (drag_and_drop_request.IsRequested())
    {        
        drag_and_drop_request.Handle();
        drag_and_drop_request.Reset();
    }
}

void SceneHierarchyWindow::SortGameObjects(
    std::vector<std::shared_ptr<engine::GameObject>>& roots_out,
    std::vector<std::shared_ptr<engine::GameObject>>& transformless_out)
{
    for (size_t i = 0; i < scene->Count(); ++i)
    {
        auto gameObject = (*scene)[i];
        auto transform = gameObject->GetComponent("GameplayCore.Components", "TransformComponent");

        if (transform == nullptr)
        {
            transformless_out.push_back(gameObject);
            continue;
        }

        auto parentProperty = transform->GetProperty("Parent");

        if (parentProperty.GetValue().has_value())
        {
            continue;
        }

        roots_out.push_back(gameObject);
    }
}

#pragma region drag&drop handlers

void SceneHierarchyWindow::WithTransformDragAndDropHandler::Handle(std::shared_ptr<engine::GameObject> source)
{
    auto sourceTransform = source->GetComponent("GameplayCore.Components", "TransformComponent");

    if (sourceTransform == nullptr)
    {
        sourceTransform = source->AddComponent("GameplayCore.Components", "TransformComponent");
    }

    auto parentProperty = sourceTransform->GetProperty("Parent");
    parentProperty.SetValue(nullptr);
}

void SceneHierarchyWindow::WithoutTransformDragAndDropHandler::Handle(std::shared_ptr<engine::GameObject> source)
{
    auto sourceTransform = source->GetComponent("GameplayCore.Components", "TransformComponent");

    if (sourceTransform != nullptr)
    {
        source->RemoveComponent(sourceTransform);
    }
}

SceneHierarchyWindow::HierarchyDragAndDropHandler::HierarchyDragAndDropHandler(std::shared_ptr<engine::GameObject> target)
    : target(target)
{}

void SceneHierarchyWindow::HierarchyDragAndDropHandler::Handle(std::shared_ptr<engine::GameObject> source) 
{
    auto targetTransform = target->GetComponent("GameplayCore.Components", "TransformComponent");                 
    auto sourceTransform = source->GetComponent("GameplayCore.Components", "TransformComponent");

    if (sourceTransform == nullptr)
    {
        sourceTransform = source->AddComponent("GameplayCore.Components", "TransformComponent");
    }

    auto parentProperty = sourceTransform->GetProperty("Parent");
    parentProperty.SetValue(*targetTransform);
}

#pragma endregion drag&drop handlers

bool SceneHierarchyWindow::DragAndDropRequest::IsRequested()
{
    return Source != nullptr && Handler != nullptr;
}

void SceneHierarchyWindow::DragAndDropRequest::Reset()
{
    Source = nullptr;
    Handler = nullptr;
}

void SceneHierarchyWindow::DragAndDropRequest::Handle()
{
    Handler->Handle(Source);
}
