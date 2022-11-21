#pragma once

#include "IEditorWindow.h"

#include <d3d11.h>
#include <filesystem>
#include <string>
#include <vector>
#include <map>

class Application;
struct FileData;
enum FileType;
class ID3D11ShaderResourceView;

class ExplorerWindow : public IEditorWindow
{
public:
    ExplorerWindow(const Application* app);
    void Draw() override;

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
    FileData(const std::string& name, const FileType type)
        :file_name(name), file_type(type)
    {
        
    }
    
    std::string file_name;
    FileType file_type;
    // Name - just filename without extension
    // Icon - for now use basic
    // Types:
    // - Plain text (first stage)
    // - Mesh
    // - Sprite
    // - Scene
    // - Prefab (maybe)
    // - Directory
    // Some serializable custom data
};

enum FileType
{
    PlainText,
    Directory,
    Scene,
    Prefab,
    Mesh,
    Texture,
    Material
};
