#include "AIEditorWindow.h"
#include "Resources/MetadataReader.h"
#include "../GameplaySystem/Object.h"

void AIEditorWindow::Draw()
{
	if (asset == nullptr || actions_list == nullptr) 
	{
		ImGui::Text("Nothing selected");
	}


}

bool AIEditorWindow::TrySelect(const std::filesystem::path& path)
{
	auto assetOpt = MetadataReader::read_internal(path);

	if (!assetOpt.has_value())
	{
		asset = nullptr;
		return false;
	}

	asset = std::make_shared<engine::Object>(std::move(assetOpt.value()));
	asset->GetType().GetMethod("Load").Invoke(*asset);
	auto actionsListOpt = asset->GetType().GetField("_actions").GetValue(*asset);

	if (!actionsListOpt.has_value()) 
	{
		actions_list = nullptr;
		return false;
	}

	actions_list = std::make_shared<engine::Object>(std::move(actionsListOpt.value()));
	return true;
}
