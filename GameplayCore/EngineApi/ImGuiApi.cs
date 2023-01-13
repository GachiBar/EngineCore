using ImGuiNET;
using System;

namespace GameplayCore.EngineApi
{
    internal static class ImGuiApi
    {
        public static void SetupImGui(IntPtr context, IntPtr allocFunc, IntPtr freeFunc)
        {
            ImGui.SetCurrentContext(context);
            ImGui.SetAllocatorFunctions(allocFunc, freeFunc);
        }
    }
}
