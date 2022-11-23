#pragma once
#include <type_traits>
#include "imgui/imgui.h"
#include "../monowrapper/monopp/mono_assembly.h"
#include "../monowrapper/monopp/mono_domain.h"

#include "ResourceData.h"
#include "../GameplaySystem/Object.h"
#include "../IEditorWindow.h"

template<class T, class U>
concept Derived = std::is_base_of_v<U, T>;

template<Derived<ResourceData> T>
class ResourceDrawer : IEditorWindow
{
public:
    void Draw() override;
    void Select(T item);
    
    static void CacheMethods(const mono::mono_assembly& assembly);

private:
    T _selected;
    std::shared_ptr<engine::Object> _selectedInstance;

    static mono::mono_method_invoker* read_;
};

template <Derived<ResourceData> T>
void ResourceDrawer<T>::Draw()
{
    ImGui::Text("Path: " + static_cast<ResourceData>(_selected).path.c_str());
    ImGui::Text("Guid: " + static_cast<ResourceData>(_selected).Guid.c_str());
    // Save changed in file every change event for test
    // TODO: Draw engine::Object instance
}

template <Derived<ResourceData> T>
void ResourceDrawer<T>::Select(T item)
{
    // Save current
    _selected = item;
    
    // Use Dotnet.MetadataReader.Read()
    auto& domain = mono::mono_domain::get_current_domain();
    mono::mono_string json(domain, absolute(static_cast<ResourceData>(_selected).path).generic_string());

    void* params[1];
    params[0] = json.get_internal_ptr();

    // Save resource in _selectedInstance
    *_selectedInstance = read_->invoke(params);
}

template <Derived<ResourceData> T>
void ResourceDrawer<T>::CacheMethods(const mono::mono_assembly& assembly)
{
    mono::mono_type type = assembly.get_type("GameplayCore.Resources", "MetadataReader");

    mono::mono_method read_method(type, "Read", 1);
    read_ = new mono::mono_method_invoker(read_method);
}
