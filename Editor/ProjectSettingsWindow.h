#pragma once


class ProjectSettingsWindow
{
public:
	void draw_imgui();


protected:
	bool draw_action_mappings_menu();
	bool draw_axis_mappings_menu();
};
