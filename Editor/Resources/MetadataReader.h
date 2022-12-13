#pragma once
#include <filesystem>
#include <coroutine>

#include "ExplorerWindow.h"
#include "FileType.h"
#include "../coroutine.h"

namespace engine 
{
    class Object;
    class Method;
    class Runtime;
}

class MetadataReader
{
public:
    static std::optional<engine::Object> read_internal(const std::filesystem::path& path);
    static void CacheMethods(engine::Runtime& runtime);

    static const std::filesystem::path AssetsPath;// = std::filesystem::current_path() / "Assets";

private:
    static engine::Method* read_;

private:
    static FileType get_file_type(const std::filesystem::directory_entry& entry);
    
public:
    static int calculate_assets_count(std::filesystem::path path);
    static Coroutine<FileData> iterate_assets_with_folder(std::filesystem::path path)
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

            if(type == Meta || type == Other || type == Directory)
                continue;
        
            FileData file_data(
                item.path(),
                type
            );

            co_yield file_data;
        }
    }

    static Coroutine<FileData> iterate_assets_recursive_by_type(std::filesystem::path path, FileType target)
    {
        if(path.empty()) co_return;
    
        for (const auto& item : std::filesystem::recursive_directory_iterator(path))
        {
            const FileType type = get_file_type(item);

            if(type != target)
                continue;
        
            FileData file_data(
                item.path(),
                type
            );

            co_yield file_data;
        }
    }
};
