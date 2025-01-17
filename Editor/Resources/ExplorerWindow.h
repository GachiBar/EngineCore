﻿#pragma once
#include <d3d11.h>
#include <filesystem>
#include <string>
#include <vector>
#include <map>

#include "libs/Delegates.h"
#include "../IEditorWindow.h"

class Application;
struct FileData;
enum FileType;
class ID3D11ShaderResourceView;

class ExplorerWindow : public IEditorWindow
{
public:
    ExplorerWindow(const Application* app);
    void Draw() override;

    DECLARE_EVENT(FileSelectedEvent, ExplorerWindow, const std::filesystem::path&)
    FileSelectedEvent FileSelected;
    
private:
    const std::vector<FileData>& get_files_data() const;
    FileType get_file_type(const std::filesystem::directory_entry& entry) const;
    void load_files_textures();
    
    bool draw_button_with_icon(const std::string& name, FileType type);
    ID3D11ShaderResourceView* get_texture(const char* filename) const;
    void on_file_picked(const std::filesystem::path& path);

    std::filesystem::path current_path;
    std::map<FileType, ID3D11ShaderResourceView*> textures;
    const Application* app;
};

struct FileData
{
    FileData() = default;
    
    FileData(const std::filesystem::path& path, const FileType type)
        :path(path), file_type(type)
    {
        
    }

    std::string filename() const {return path.filename().generic_string();}

    std::filesystem::path path;
    FileType file_type;
};
