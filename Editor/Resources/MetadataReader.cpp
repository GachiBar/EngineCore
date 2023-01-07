#include "MetadataReader.h"
#include <filesystem>

#include "ExplorerWindow.h"

#include "../monowrapper/monopp/mono_string.h"
#include "../../GameplaySystem/Object.h"
#include "../../GameplaySystem/Method.h"
#include "../../GameplaySystem/Runtime.h"

engine::Method* MetadataReader::read_ = nullptr;
engine::Method* MetadataReader::create_material = nullptr;
engine::Method* MetadataReader::create_ai_actions = nullptr;
const std::filesystem::path MetadataReader::AssetsPath = std::filesystem::current_path() / "Assets";

std::optional<engine::Object> MetadataReader::read_internal(const std::filesystem::path& path)
{
    auto& domain = engine::Runtime::GetCurrentRuntime().GetDomain();
    mono::mono_string path_string(domain, absolute(path).generic_string());

    void* params[1];
    params[0] = path_string.get_internal_ptr();

    return read_->Invoke(params);
}

void MetadataReader::create_material_internal(const std::filesystem::path& base_path)
{
    auto& domain = engine::Runtime::GetCurrentRuntime().GetDomain();
    mono::mono_string path_string(domain, absolute(base_path).generic_string());

    void* params[1];
    params[0] = path_string.get_internal_ptr();

    create_material->Invoke(params);
}

void MetadataReader::create_ai_actions_internal(const std::filesystem::path& base_path)
{
    auto& domain = engine::Runtime::GetCurrentRuntime().GetDomain();
    mono::mono_string path_string(domain, absolute(base_path).generic_string());

    void* params[1];
    params[0] = path_string.get_internal_ptr();

    create_ai_actions->Invoke(params);
}

void MetadataReader::CacheMethods(engine::Runtime& runtime)
{
    auto type = runtime.GetType("GameplayCore.Resources", "MetadataReader");
    read_ = new engine::Method(type, "Read", 1);
    create_material = new engine::Method(type, "CreateMaterial", 1);
    create_ai_actions = new engine::Method(type, "CreateAIActions", 1);
}

FileType MetadataReader::get_file_type(const std::filesystem::directory_entry& entry)
{
    return GetTypeByPath(entry.path());
}

FileType MetadataReader::GetTypeByClassName(const std::string& classname)
{
    if (classname == "SceneAsset")
        return FileType::Scene;
    else if (classname == "Prefab")
        return FileType::Prefab;
    else if (classname == "MeshAsset")
        return FileType::Mesh;
    else if (classname == "MaterialAsset")
        return FileType::Material;
    else if (classname == "TextureAsset")
        return FileType::Texture;
    else if (classname == "TextAsset")
        return FileType::PlainText;
    else if (classname == "AIActionsAsset")
        return FileType::AIActions;

    return FileType::Other;
}

FileType MetadataReader::GetTypeByPath(const std::filesystem::path& path)
{
    if (std::filesystem::is_directory(path))
        return FileType::Directory;

    // Actually there is should be .meta search, but let it be here for now
    if (std::filesystem::is_regular_file(path))
    {
        const std::string extension = path.filename().extension().generic_string();
        if (extension == ".scene")
            return FileType::Scene;
        else if (extension == ".prefab")
            return FileType::Prefab;
        else if (extension == ".mesh" || extension == ".fbx" || extension == ".obj")
            return FileType::Mesh;
        else if (extension == ".material")
            return FileType::Material;
        else if (extension == ".texture" || extension == ".jpg" || extension == ".png")
            return FileType::Texture;
        else if (extension == ".meta")
            return FileType::Meta;
        else if (extension == ".txt")
            return FileType::PlainText;
        else if (extension == ".ai")
            return FileType::AIActions;
    }

    return FileType::Other;
}

int MetadataReader::calculate_assets_count(std::filesystem::path path)
{
    if(path.empty())
        return 0;

    int count = 0;
    for (const auto& item : std::filesystem::recursive_directory_iterator(path))
    {
        const FileType type = get_file_type(item);

        if(type == Meta || type == Other || type == Directory)
            continue;
        
        count++;
    }

    return count;
}
