#pragma once
#include <filesystem>

struct ResourceData;

class MetadataReader
{
public:
    const ResourceData read(const std::filesystem::path& path);
    const ResourceData create(const std::filesystem::path& path);
    bool has_metadata(const std::filesystem::path& path);
};
