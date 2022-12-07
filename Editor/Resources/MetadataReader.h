#pragma once
#include <filesystem>
#include <coroutine>

#include "ExplorerWindow.h"
#include "FileType.h"
#include "../coroutine.h"

namespace mono
{
    class mono_object;
    class mono_assembly;
    class mono_method_invoker;
}

class MetadataReader
{
public:
    static std::optional<mono::mono_object> read_internal(const std::filesystem::path& path);
    static void CacheMethods(const mono::mono_assembly& assembly);

    static const std::filesystem::path AssetsPath;// = std::filesystem::current_path() / "Assets";

private:
    static mono::mono_method_invoker* read_;

private:
    static FileType get_file_type(const std::filesystem::directory_entry& entry);
    
public:
    static Coroutine<FileData> iterate_assets(std::filesystem::path path)
    {
        if(path.empty()) co_return;
    
        for (const auto& item : std::filesystem::directory_iterator(path))
        {
            const FileType type = get_file_type(item);

            if(type == Meta || type == Other)
                continue;
        
            FileData file_data(
                item.path(),
                type
            );

            co_yield file_data;
        }
    }

    static Coroutine<FileData> iterate_assets_recursive(std::filesystem::path path)
    {
        if(path.empty()) co_return;
    
        for (const auto& item : std::filesystem::recursive_directory_iterator(path))
        {
            const FileType type = get_file_type(item);

            if(type == Meta || type == Other)
                continue;
        
            FileData file_data(
                item.path(),
                type
            );

            co_yield file_data;
        }
    }

    static int calculate_assets_count(std::filesystem::path path)
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
};
