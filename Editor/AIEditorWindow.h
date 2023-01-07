#pragma once

#include "IEditorWindow.h"
#include "../GameplaySystem/Object.h"

#include <filesystem>

class AIEditorWindow : public IEditorWindow
{
public:
    void Draw() override;
    bool TrySelect(const std::filesystem::path& path);

private:
    std::shared_ptr<engine::Object> asset;
    std::shared_ptr<engine::Object> actions_list;
};
