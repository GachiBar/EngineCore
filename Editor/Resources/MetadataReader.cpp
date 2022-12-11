#include "MetadataReader.h"
#include <filesystem>

#include "ExplorerWindow.h"

#include "../monowrapper/monopp/mono_string.h"
#include "../../GameplaySystem/Object.h"
#include "../../GameplaySystem/Method.h"
#include "../../GameplaySystem/Runtime.h"

engine::Method* MetadataReader::read_ = nullptr;
const std::filesystem::path MetadataReader::AssetsPath = std::filesystem::current_path() / "Assets";

std::optional<engine::Object> MetadataReader::read_internal(const std::filesystem::path& path)
{
    auto& domain = engine::Runtime::GetCurrentRuntime().GetDomain();
    mono::mono_string path_string(domain, absolute(path).generic_string());

    void* params[1];
    params[0] = path_string.get_internal_ptr();

    return read_->Invoke(params);
}

void MetadataReader::CacheMethods(engine::Runtime& runtime)
{
    auto type = runtime.GetType("GameplayCore.Resources", "MetadataReader");
    read_ = new engine::Method(type, "Read", 1);
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
