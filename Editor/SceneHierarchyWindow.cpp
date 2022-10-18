#include "SceneHierarchyWindow.h"

#include "imgui/imgui.h"

void SceneHierarchyWindow::draw_imgui(engine::CSharpObject& scene)
{
    ImGui::Begin("Scene Hierarchy");
    /*
    List<engine::CSharpObject> gameObjects = scene.getGameObjects();
    int index = 0;
    for (engine::CSharpObject& obj : gameObjects) {
        bool treeNodeOpen = DoTreeNode(obj, index);
        if (treeNodeOpen) {
            ImGui::TreePop();
        }
        index++;
    }
    */
    ImGui::End();
}

bool SceneHierarchyWindow::DoTreeNode(engine::CSharpObject& go, int index)
{
    return false;
}
