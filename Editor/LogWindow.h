#pragma once

#include <string>

#include "IEditorWindow.h"
#include "imgui/imgui.h"
#include "../Core/libs/loguru/loguru.hpp"

class LogWindow : public IEditorWindow
{
public:
	LogWindow();

	bool bIsOpen = false;
	void Draw() override;
	void Clear();

	void OnLogMessageAdded(loguru::Message const& InMessage);

protected:
	void AddLog(std::string const& InMessage, loguru::Verbosity InVerbosityLevel);

	void Draw_Implementation(const char* title, bool* p_open = nullptr);

	ImGuiTextBuffer     Buf;
	ImGuiTextFilter     Filter;
	ImVector<int>       LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
	ImVector<loguru::Verbosity> LineVerbosity;
	bool                AutoScroll;  // Keep scrolling if already at the bottom.
};
