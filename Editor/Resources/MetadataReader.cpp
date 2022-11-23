#include "MetadataReader.h"
#include "ResourceData.h"

#include <filesystem>

const ResourceData MetadataReader::read(const std::filesystem::path& path)
{
    const std::string filename = (path.stem().generic_string() + ".meta");
    std::filesystem::path metafile_path = path.parent_path() / filename;
    if (exists(metafile_path))
    {
        // return resource struct: 
    }
    else
    {
        // create valid basic .meta file with correct type
    }

    return {};
}
