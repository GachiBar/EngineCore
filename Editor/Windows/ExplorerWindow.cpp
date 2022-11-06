#include "ExplorerWindow.h"
#include <d3d11.h>
#include <iostream>

#include "imgui/imgui.h"

namespace fs = std::filesystem;

ExplorerWindow::ExplorerWindow()
{
    current_path = fs::relative(fs::current_path(), fs::current_path());
    load_files_textures();
}

void ExplorerWindow::draw()
{
    ImGui::Begin("Explorer");
    std::string path = "Current path : ";
    path += current_path.relative_path().generic_string();
    // path += current_path.generic_string();
    ImGui::Text(path.c_str());

    //ImGui::End();
    //return;

    
    if (current_path.has_parent_path())
    {
        const std::filesystem::path parent = current_path.parent_path();
        if (draw_button_with_icon("..", Directory))
        {
            current_path = parent;
        }
    }

    for (const auto& entry : std::filesystem::directory_iterator(current_path))
    {
        const std::filesystem::path& file_path = entry.path();
        const std::string entryName = std::filesystem::relative(file_path, current_path).string();
        if (entry.is_directory())
        {
            if (draw_button_with_icon(entryName, Directory))
            {
                current_path = file_path;
                break;
            }
        }
        else if (entry.is_regular_file())
        {
            if (draw_button_with_icon(entryName.c_str(), PlainText))
            {
                on_file_picked(file_path);
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

const std::vector<FileData>& ExplorerWindow::get_files_data() const
{
    std::vector<FileData> data;

    for (const auto& item : fs::directory_iterator(current_path))
    {
        FileType type;

        if (item.is_directory())
            type = FileType::Directory;
        else if (item.is_regular_file())
            type = FileType::PlainText;
        else
            continue;


        FileData file_data(
            item.path().filename().generic_string(),
            type
        );

        data.push_back(file_data);
    }

    return data;
}

ID3D11ShaderResourceView* ExplorerWindow::get_texture(const char* filename)
{
    return nullptr;
}

void ExplorerWindow::load_files_textures()
{
    textures[FileType::PlainText] = get_texture("Icons/TextIcon.png");
    textures[FileType::Directory] = get_texture("Icons/FolderIcon.png");
}

void ExplorerWindow::on_file_picked(const std::filesystem::path& path)
{
    std::cout << "Picked file: " << path.generic_string() << std::endl; 
}