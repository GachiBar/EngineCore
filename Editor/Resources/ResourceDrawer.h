#pragma once
#include <type_traits>
#include "imgui/imgui.h"
#include "../monowrapper/monopp/mono_assembly.h"
#include "../monowrapper/monopp/mono_domain.h"

#include "ResourceData.h"
#include "../GameplaySystem/Object.h"
#include "../IEditorWindow.h"
#include "../ObjectDrawer.h"

template<class T, class U>
concept Derived = std::is_base_of_v<U, T>;

//template<Derived<ResourceData> T>
class ResourceDrawer : IEditorWindow
{
public:
    void Draw() override;
    void Select(ResourceData item);
    void TrySelect(const std::filesystem::path& path); 
    
    static void CacheMethods(const mono::mono_assembly& assembly);

private:
    ResourceData _selected;
    std::shared_ptr<engine::Object> _selectedInstance;

    ObjectDrawer object_drawer;
    static mono::mono_method_invoker* read_;
};
