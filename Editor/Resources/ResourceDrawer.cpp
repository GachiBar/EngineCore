#include "ResourceDrawer.h"
#include <format>

mono::mono_method_invoker* ResourceDrawer::read_ = nullptr;

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
    // Save changed in file every change event for test
    // TODO: Draw engine::Object instance
    ImGui::End();
}

void ResourceDrawer::Select(ResourceData item)
{
    // Save current
    _selected = item;
    
    // Use Dotnet.MetadataReader.Read()
    auto& domain = mono::mono_domain::get_current_domain();
    mono::mono_string json(domain, absolute(_selected.path).generic_string());

    void* params[1];
    params[0] = json.get_internal_ptr();

    // Save resource in _selectedInstance
    mono::mono_object object(read_->invoke(params).value());
    _selectedInstance = std::make_shared<engine::Object>(engine::Object(object));
}

void ResourceDrawer::TrySelect(const std::filesystem::path& path)
{
    try
    {
        auto& domain = mono::mono_domain::get_current_domain();
        printf("Reading metadata of %s\n", path.generic_string().c_str());
        mono::mono_string json(domain, absolute(path).generic_string());

        void* params[1];
        params[0] = json.get_internal_ptr();

        // Save resource in _selectedInstance
        mono::mono_object object(read_->invoke(params).value());
        _selectedInstance = std::make_shared<engine::Object>(engine::Object(object));
    }
    catch (std::exception& e)
    {
        printf("Can't read metadata of %s: %s\n", path.generic_string().c_str(), e.what());
        return;
    }

    _selected = {path.generic_string(), "Doesn't matter"};
}

void ResourceDrawer::CacheMethods(const mono::mono_assembly& assembly)
{
    mono::mono_type type = assembly.get_type("GameplayCore.Resources", "MetadataReader");

    mono::mono_method read_method(type, "Read", 1);
    read_ = new mono::mono_method_invoker(read_method);
}
