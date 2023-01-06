#include "PropertyWindow.h"

#include "imgui/imgui.h"

#include "../GameplaySystem/GameObject.h"
#include "Resources/MaterialsEditor.h"
#include "GameObjectInspectorWindow.h"
#include "AIEditorWindow.h"
#include "Resources/MetadataReader.h"

PropertyWindow::PropertyWindow(std::shared_ptr<GameObjectInspectorWindow> game_object,
                               std::shared_ptr<MaterialsEditor> resource, std::shared_ptr<AIEditorWindow> ai)
{
    game_object_drawer_ = game_object;
    materials_drawer_ = resource;
    ai_editor_drawer_ = ai;
}

void PropertyWindow::Draw()
{
    bool p_open = true;

    ImGuiWindowFlags flags = GetFlags();
    if (current_drawer != nullptr)
        flags |= current_drawer->GetFlags();
    
    ImGui::Begin("PropertyWindow", &p_open, flags);

    if (current_drawer != nullptr)
        current_drawer->Draw();

    ImGui::End();
}

void PropertyWindow::SelectGameObject(std::shared_ptr<engine::GameObject> game_object)
{
    game_object_drawer_->SetGameObject(game_object);
    current_drawer = dynamic_cast<IEditorWindow*>(game_object_drawer_.get());
}

void PropertyWindow::SelectResource(const std::filesystem::path& path)
{
    // TODO: Add Ai
    if(MetadataReader::GetTypeByPath(path) == Material)
    {
        materials_drawer_->TrySelect(path);
        current_drawer = dynamic_cast<IEditorWindow*>(materials_drawer_.get());    
    }
}
