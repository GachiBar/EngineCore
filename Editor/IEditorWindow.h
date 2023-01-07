#pragma once
#include "imgui/imgui.h"

class IEditorWindow {
public:
	virtual ~IEditorWindow() = default;
	virtual void Draw() = 0;

	virtual ImGuiWindowFlags GetFlags() {return ImGuiWindowFlags_None;}
};
