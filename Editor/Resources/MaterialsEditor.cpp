#include "MaterialsEditor.h"

#include <format>
#include "imgui/imgui.h"

#include "MetadataReader.h"
#include "../../GameplaySystem/Object.h"

void MaterialsEditor::Draw()
{
    if(_selectedInstance == nullptr)
    {
        ImGui::Text("Nothing selected");
        ImGui::End();
        return;
    }
    
    std::vector<std::string> modifiedFields;
    ImGui::Text(std::format("{}{}", "Path: ", _selected.path.generic_string()).c_str());
    ImGui::Text(std::format("{}{}", "Guid: ", _selected.Guid).c_str());

    // Draw material field
    if(_material != nullptr && object_drawer.DrawObject(*_material, modifiedFields))
    {
        // If field has changed -> apply settings to file
        _selectedInstance->GetType().GetMethod("SaveData").Invoke(*_selectedInstance);
    }
}

void MaterialsEditor::TrySelect(const std::filesystem::path& path)
{
    try
    {
        printf("Reading metadata of %s\n", path.generic_string().c_str());
        std::optional<engine::Object> resource = MetadataReader::read_internal(path);
        
        // Save resource in _selectedInstance
        _selectedInstance = std::make_shared<engine::Object>(std::move(resource.value()));

        // Load Material from file
        _selectedInstance->GetType().GetMethod("LoadData").Invoke(*_selectedInstance);

        auto raw_pointer = _selectedInstance->GetType().GetField("_material").GetValue(*_selectedInstance);
        if(raw_pointer.has_value())
            _material = std::make_shared<engine::Object>(raw_pointer.value());
        else
        {
            _material == nullptr;
        }
    }
    catch (std::exception& e)
    {
        printf("Can't read metadata of %s: %s\n", path.generic_string().c_str(), e.what());
        return;
    }

    _selected = {path.generic_string(), path.generic_string()};
}
