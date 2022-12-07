#include "ResourceDrawer.h"

#include <format>
#include "imgui/imgui.h"

#include "MetadataReader.h"

void ResourceDrawer::Draw()
{
    ImGui::Begin("Resource Drawer");
    if(_selectedInstance == nullptr)
    {
        ImGui::Text("Nothing selected");
        ImGui::End();
        return;
    }
    
    ImGui::Text(std::format("{}{}", "Path: ", _selected.path.generic_string()).c_str());
    ImGui::Text(std::format("{}{}", "Guid: ", _selected.Guid).c_str());
    object_drawer.DrawObject(*_selectedInstance);
    ImGui::End();
}

void ResourceDrawer::TrySelect(const std::filesystem::path& path)
{
    try
    {
        printf("Reading metadata of %s\n", path.generic_string().c_str());
        std::optional<mono::mono_object> raw_resource = MetadataReader::read_internal(path);
        
        // Save resource in _selectedInstance
        mono::mono_object object(raw_resource.value());
        _selectedInstance = std::make_shared<engine::Object>(engine::Object(object));
    }
    catch (std::exception& e)
    {
        printf("Can't read metadata of %s: %s\n", path.generic_string().c_str(), e.what());
        return;
    }

    _selected = {path.generic_string(), path.generic_string()};
}
