#pragma once

#include "IEditorWindow.h"

class ConfigReaderWriter;

class ProjectSettingsWindow : public IEditorWindow
{
public:
	ProjectSettingsWindow();
	~ProjectSettingsWindow() override;
	void Draw() override;

protected:
	ConfigReaderWriter* config_reader_writer;

	bool draw_action_mappings_menu();
	bool draw_axis_mappings_menu();
};
