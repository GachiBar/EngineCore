#pragma once
#include <type_traits>

#include "ResourceData.h"
#include "../GameplaySystem/Object.h"
#include "../IEditorWindow.h"
#include "../ObjectDrawer.h"

template<class T, class U>
concept Derived = std::is_base_of_v<U, T>;

class MaterialsEditor : public IEditorWindow
{
public:
    void Draw() override;
    void TrySelect(const std::filesystem::path& path);

private:
    ResourceData _selected;
    std::shared_ptr<engine::Object> _selectedInstance;

    ObjectDrawer object_drawer;
};
