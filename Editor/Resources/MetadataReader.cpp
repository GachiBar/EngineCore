#include "MetadataReader.h"
#include <filesystem>

#include "ExplorerWindow.h"

#include "../monowrapper/monopp/mono_assembly.h"
#include "../monowrapper/monopp/mono_domain.h"
#include "../monowrapper/monopp/mono_object.h"
#include "../monowrapper/monopp/mono_string.h"
#include "../monowrapper/monopp/mono_method_invoker.h"

mono::mono_method_invoker* MetadataReader::read_ = nullptr;
const std::filesystem::path MetadataReader::AssetsPath = std::filesystem::current_path() / "Assets";

std::optional<mono::mono_object> MetadataReader::read_internal(const std::filesystem::path& path)
{
    auto& domain = mono::mono_domain::get_current_domain();
    mono::mono_string path_string(domain, absolute(path).generic_string());

    void* params[1];
    params[0] = path_string.get_internal_ptr();
    
    auto value = read_->invoke(params);
    return value;
}

void MetadataReader::CacheMethods(const mono::mono_assembly& assembly)
{
    mono::mono_type type = assembly.get_type("GameplayCore.Resources", "MetadataReader");

    mono::mono_method read_method(type, "Read", 1);
    read_ = new mono::mono_method_invoker(read_method);
}

FileType MetadataReader::get_file_type(const std::filesystem::directory_entry& entry)
{
    if (entry.is_directory())
        return FileType::Directory;

    // Actually there is should be .meta search, but let it be here for now
    if (entry.is_regular_file())
    {
        std::string extension = entry.path().filename().extension().generic_string();
        if(extension == ".scene")
            return FileType::Scene;
        else if (extension == ".prefab")
            return FileType::Prefab;
        else if (extension == ".mesh")
            return FileType::Mesh;
        else if (extension == ".material")
            return FileType::Material;
        else if (extension == ".texture" || extension == ".jpg" || extension == ".png")
            return FileType::Texture;
        else if (extension == ".meta")
            return FileType::Meta;
        else if (extension == ".txt")
            return FileType::PlainText;
    }

    // std::cout << "Can't handle non-directory and non-file!\n";
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
