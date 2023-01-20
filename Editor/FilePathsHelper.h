#pragma once

#include <string>

class FilePahtsHelper 
{
public:
    FilePahtsHelper() = delete;

    static std::wstring BaseName(const std::wstring& path, const std::wstring& delims = L"/\\")
    {
        return path.substr(path.find_last_of(delims) + 1);
    }

    static std::wstring RemoveExtension(const std::wstring& filename)
    {
        std::wstring::size_type const p(filename.find_last_of('.'));
        return p > 0 && p != std::wstring::npos ? filename.substr(0, p) : filename;
    }

    static std::string BaseName(const std::string& path, const std::string& delims = "/\\")
    {
        return path.substr(path.find_last_of(delims) + 1);
    }

    static std::string RemoveExtension(const std::string& filename)
    {
        std::string::size_type const p(filename.find_last_of('.'));
        return p > 0 && p != std::string::npos ? filename.substr(0, p) : filename;
    }

    static std::string ws2s(const std::wstring& wstr)
    {
        // TODO: It's not a solution, but ok...
        return std::string(wstr.begin(), wstr.end());
    }

    static std::wstring s2ws(const std::string& str)
    {
        // TODO: It's not a solution, but ok...
        return std::wstring(str.begin(), str.end());
    }
};

