#include "ExplorerWindow.h"
#include <d3d11.h>
#include <iostream>

#include "imgui/imgui.h"
#include "FileType.h"
#include "MetadataReader.h"
#include "../Application.h"
#include "../GameplaySystem/Engine.h"

namespace fs = std::filesystem;

ExplorerWindow::ExplorerWindow(const Application* app)
{
    this->app = app;
    current_path = fs::relative(fs::current_path() / "Assets", fs::current_path());
    load_files_textures();
}

void ExplorerWindow::Draw()
{
    bool p_open = true;
    ImGui::Begin("Explorer", &p_open, ImGuiWindowFlags_MenuBar);
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Create"))
        {
            if (ImGui::MenuItem("Material"))
            {
                // Create material at current_path by calling MetadataReader::CreateMaterial(object) perhaps
                MetadataReader::create_material_internal(current_path);
            }
            if (ImGui::MenuItem("AIActions")) 
            {
                MetadataReader::create_ai_actions_internal(current_path);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    
    std::string path = "Current path : ";
    path += current_path.relative_path().generic_string();
    ImGui::Text(path.c_str());
    
    if (current_path.has_parent_path())
    {
        const std::filesystem::path parent = current_path.parent_path();
        if (draw_button_with_icon("..", Directory))
        {
            current_path = parent;
            ImGui::End();
            return;
        }
    }

    auto iterator = MetadataReader::iterate_assets_with_folder(current_path);
    for (int j = 0; iterator; j++)
    {
        FileData data = iterator();
        if(draw_button_with_icon(data.filename(), data.file_type))
        {
            if(data.file_type == Directory)
            {
                current_path = data.path;
                break;
            }
            else
            {
                on_file_picked(data.path);
                break;
            }
        }
    }

    ImGui::End();
}

bool ExplorerWindow::draw_button_with_icon(const std::string& name, FileType type)
{
    ImGui::PushID(name.c_str());
    bool clicked = false;

    const ImTextureID imTex = textures[type];
    clicked |= ImGui::ImageButton(imTex, ImVec2(16, 16));
    ImGui::SameLine();

    clicked |= ImGui::Button(name.c_str());
    ImGui::PopID();
    return clicked;
};

ID3D11ShaderResourceView* ExplorerWindow::get_texture(const char* filename) const
{
    RenderDevice& device = app->GetEngine()->GetRenderer();
    
    size_t id = std::strlen(filename);
    device.RegisterTexture(id, filename);
    const RenderDevice::Texture texture = device.GetTexture(id);
    
    return static_cast<ID3D11ShaderResourceView*>(texture.texture);
}

void ExplorerWindow::load_files_textures()
{
    const fs::path icons_path = fs::current_path() / "vendor" / "Icons";
    textures[FileType::PlainText] = get_texture(absolute((icons_path / "files.png")).generic_string().c_str());
    textures[FileType::Directory] = get_texture((icons_path / "folder.png").generic_string().c_str());
}

void ExplorerWindow::on_file_picked(const std::filesystem::path& path)
{
    std::cout << "Picked file: " << path.generic_string() << std::endl;
    FileSelected.Broadcast(path);
    // path.
    // Get meta data
    // If there is no meta data -> generate
    // Pass resource to drawer
}