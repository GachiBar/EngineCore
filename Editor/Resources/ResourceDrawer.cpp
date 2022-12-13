#include "ResourceDrawer.h"

#include <format>
#include "imgui/imgui.h"

#include "MetadataReader.h"
#include "../../GameplaySystem/Object.h"

void ResourceDrawer::Draw()
{
    ImGui::Begin("Resource Drawer");
    if(_selectedInstance == nullptr)
    {
        ImGui::Text("Nothing selected");
        ImGui::End();
        return;
    }
    
    std::vector<std::string> modifiedFields;
    ImGui::Text(std::format("{}{}", "Path: ", _selected.path.generic_string()).c_str());
    ImGui::Text(std::format("{}{}", "Guid: ", _selected.Guid).c_str());
    object_drawer.DrawObject(*_selectedInstance, modifiedFields);
    ImGui::End();
}

void ResourceDrawer::TrySelect(const std::filesystem::path& path)
{
    try
    {
        printf("Reading metadata of %s\n", path.generic_string().c_str());
        std::optional<engine::Object> resource = MetadataReader::read_internal(path);
        
        // Save resource in _selectedInstance
        _selectedInstance = std::make_shared<engine::Object>(std::move(resource.value()));
    }
    catch (std::exception& e)
    {
        printf("Can't read metadata of %s: %s\n", path.generic_string().c_str(), e.what());
        return;
    }

    _selected = {path.generic_string(), path.generic_string()};
}
