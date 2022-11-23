#include "LogWindow.h"
#include "libs/imgui_sugar.hpp"
#include "../Logger/LogManager.h"

LogWindow::LogWindow()
{
    AutoScroll = true;
    LogManager::getInstance().LogMessageAdded.AddRaw(this, &LogWindow::OnLogMessageAdded);
    Clear();
}

void LogWindow::Draw()
{
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    
    Draw_Implementation("Log", &bIsOpen);
}

void LogWindow::Clear()
{
    Buf.clear();
    LineOffsets.clear();
    LineOffsets.push_back(0);
}

void LogWindow::OnLogMessageAdded(loguru::Message const & InMessage)
{
    AddLog(std::string(InMessage.preamble) + std::string(InMessage.message), InMessage.verbosity);
}

void LogWindow::AddLog(std::string const& InMessage, loguru::Verbosity InVerbosityLevel)
{
    if (InMessage.empty())
        return;

    Buf.appendf("%s\n", InMessage.c_str());
    LineOffsets.push_back(Buf.size());
    LineVerbosity.push_back(InVerbosityLevel);
}

void LogWindow::Draw_Implementation(const char* title, bool* p_open)
{
    if (!ImGui::Begin(title, p_open))
    {
        ImGui::End();
        return;
    }

    // Options menu
    if (ImGui::BeginPopup("Options"))
    {
        ImGui::Checkbox("Auto-scroll", &AutoScroll);
        ImGui::EndPopup();
    }

    // Main window
    if (ImGui::Button("Options"))
        ImGui::OpenPopup("Options");
    ImGui::SameLine();
    bool clear = ImGui::Button("Clear");
    ImGui::SameLine();

    Filter.Draw("Filter", -100.0f);

    ImGui::Separator();

    if (ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar))
    {
        if (clear)
            Clear();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        const char* buf = Buf.begin();
        const char* buf_end = Buf.end();
        if (Filter.IsActive())
        {
            for (int line_no = 0; line_no < LineOffsets.Size-1; line_no++)
            {
                const char* line_start = buf + LineOffsets[line_no];
                const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                if (Filter.PassFilter(line_start, line_end))
                {
	                const auto LogColor = LogManager::getInstance().GetLevelLogColor(LineVerbosity[line_no]);
                    ImVec4 color = { LogColor.x,LogColor.y ,LogColor.z ,LogColor.w};
                    ImGui::PushStyleColor(ImGuiCol_Text, color);
                    ImGui::TextUnformatted(line_start, line_end);
                    ImGui::PopStyleColor();
                }
            }
        }
        else
        {
            ImGuiListClipper clipper;
            clipper.Begin(LineOffsets.Size-1);
            while (clipper.Step())
            {
                for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
                {
                    const char* line_start = buf + LineOffsets[line_no];
                    const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                    const auto LogColor = LogManager::getInstance().GetLevelLogColor(LineVerbosity[line_no]);
                    ImVec4 color = { LogColor.x,LogColor.y ,LogColor.z ,LogColor.w };
                    ImGui::PushStyleColor(ImGuiCol_Text, color);
                    ImGui::TextUnformatted(line_start, line_end);
                    ImGui::PopStyleColor();
                }
            }
            clipper.End();
        }
        ImGui::PopStyleVar();

        if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);
    }
    ImGui::EndChild();
    ImGui::End();
}
