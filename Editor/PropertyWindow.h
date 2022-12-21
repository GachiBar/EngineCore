#pragma once
#include <filesystem>
#include <memory>

#include "IEditorWindow.h"

namespace engine{class GameObject;}

class AIEditorWindow;
class MaterialsEditor;
class GameObjectInspectorWindow;

class PropertyWindow : public IEditorWindow
{
public:
    PropertyWindow(std::shared_ptr<GameObjectInspectorWindow> game_object,
        std::shared_ptr<MaterialsEditor> resource,
        std::shared_ptr<AIEditorWindow> ai);
    
    void Draw() override;
    void SelectGameObject(std::shared_ptr<engine::GameObject> game_object);
    void SelectResource(const std::filesystem::path& path);

private:
    IEditorWindow* current_drawer = nullptr;

    std::shared_ptr<GameObjectInspectorWindow> game_object_drawer_;
    std::shared_ptr<MaterialsEditor> materials_drawer_;
    std::shared_ptr<AIEditorWindow> ai_editor_drawer_;
};
