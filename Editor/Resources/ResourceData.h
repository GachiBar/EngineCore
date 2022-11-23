#pragma once
#include <filesystem>
#include <string>

struct ResourceData
{
    std::string Guid;
    std::filesystem::path path;
};
