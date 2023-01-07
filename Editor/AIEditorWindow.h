#pragma once

#include "IEditorWindow.h"
#include "../GameplaySystem/Object.h"

#include <filesystem>

class AIEditorWindow : public IEditorWindow
{
public:
    AIEditorWindow();
    ~AIEditorWindow();

    void Draw() override;
    bool TrySelect(const std::filesystem::path& path);

private:
    std::shared_ptr<engine::Object> asset;
    std::shared_ptr<engine::Object> actions_list;

    const char** available_actions_items;
    size_t avaliable_actions_count;

    std::vector<std::string> actions_names;

    void SaveAsset();

    size_t GetCount();
    engine::Object GetAction(size_t index);
    void AddAction(engine::Object action);
    void RemoveAction(engine::Object action);

    void DrawExistingActions();
    void DrawAddActionPanel();

    void CacheActionsData();
    void FindAvaliableActions();
};
