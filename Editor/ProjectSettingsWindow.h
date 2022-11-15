#pragma once

#include "IEditorWindow.h"

class ProjectSettingsWindow : public IEditorWindow
{
public:
	void Draw() override;

protected:
	bool draw_action_mappings_menu();
	bool draw_axis_mappings_menu();
};
