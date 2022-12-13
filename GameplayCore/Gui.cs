using System;
using System.Collections.Generic;
using System.Diagnostics.SymbolStore;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using GameplayCore.Mathematics;

namespace GameplayCore
{
	public enum ImGuiWindowFlags
	{
		None = 0,
		NoTitleBar = 1 << 0,   // Disable title-bar
		NoResize = 1 << 1,   // Disable user resizing with the lower-right grip
		NoMove = 1 << 2,   // Disable user moving the window
		NoScrollbar = 1 << 3,   // Disable scrollbars (window can still scroll with mouse or programmatically)
		NoScrollWithMouse = 1 << 4,   // Disable user vertically scrolling with mouse wheel. On child window, mouse wheel will be forwarded to the parent unless NoScrollbar is also set.
		NoCollapse = 1 << 5,   // Disable user collapsing window by double-clicking on it. Also referred to as Window Menu Button (e.g. within a docking node).
		AlwaysAutoResize = 1 << 6,   // Resize every window to its content every frame
		NoBackground = 1 << 7,   // Disable drawing background color (WindowBg, etc.) and outside border. Similar as using SetNextWindowBgAlpha(0.0f).
		NoSavedSettings = 1 << 8,   // Never load/save settings in .ini file
		NoMouseInputs = 1 << 9,   // Disable catching mouse, hovering test with pass through.
		MenuBar = 1 << 10,  // Has a menu-bar
		HorizontalScrollbar = 1 << 11,  // Allow horizontal scrollbar to appear (off by default). You may use SetNextWindowContentSize(ImVec2(width,0.0f)); prior to calling Begin() to specify width. Read code in imgui_demo in the "Horizontal Scrolling" section.
		NoFocusOnAppearing = 1 << 12,  // Disable taking focus when transitioning from hidden to visible state
		NoBringToFrontOnFocus = 1 << 13,  // Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)
		AlwaysVerticalScrollbar = 1 << 14,  // Always show vertical scrollbar (even if ContentSize.y < Size.y)
		AlwaysHorizontalScrollbar = 1 << 15,  // Always show horizontal scrollbar (even if ContentSize.x < Size.x)
		AlwaysUseWindowPadding = 1 << 16,  // Ensure child windows without border uses style.WindowPadding (ignored by default for non-bordered child windows, because more convenient)
		NoNavInputs = 1 << 18,  // No gamepad/keyboard navigation within the window
		NoNavFocus = 1 << 19,  // No focusing toward this window with gamepad/keyboard navigation (e.g. skipped by CTRL+TAB)
		UnsavedDocument = 1 << 20,  // Display a dot next to the title. When used in a tab/docking context, tab is selected when clicking the X + closure is not assumed (will wait for user to stop submitting the tab). Otherwise closure is assumed when pressing the X, so if you keep submitting the tab may reappear at end of tab bar.
		NoDocking = 1 << 21,  // Disable docking of this window

		NoNav = NoNavInputs | NoNavFocus,
		NoDecoration = NoTitleBar | NoResize | NoScrollbar | NoCollapse,
		NoInputs = NoMouseInputs | NoNavInputs | NoNavFocus,

		// [Internal]
		NavFlattened = 1 << 23,  // [BETA] On child window: allow gamepad/keyboard navigation to cross over parent border to this child or between sibling child windows.
		ChildWindow = 1 << 24,  // Don't use! For internal use by BeginChild()
		Tooltip = 1 << 25,  // Don't use! For internal use by BeginTooltip()
		Popup = 1 << 26,  // Don't use! For internal use by BeginPopup()
		Modal = 1 << 27,  // Don't use! For internal use by BeginPopupModal()
		ChildMenu = 1 << 28,  // Don't use! For internal use by BeginMenu()
		DockNodeHost = 1 << 29,  // Don't use! For internal use by Begin()/NewFrame()
	};

	public static class Gui
	{
		public static void LabelText(string text)
		{
			ImGuiWindowFlags imGuiWindowFlags_ = ImGuiWindowFlags.None | ImGuiWindowFlags.NoScrollWithMouse;

			Internal_LabelText(text);
		}

		public static void Text(string text)
		{
			Internal_Text(text);
		}

		public static bool Button(string text, Vector2 pos)
		{
			return Internal_Button(text, pos.X, pos.Y);
		}

		public static bool Begin(string name, bool open, ImGuiWindowFlags flags)
		{
			return Internal_Begin(name,open, (int)flags);
		}

		public static bool BeginChild(string str_id, Vector2 size_arg, bool border, ImGuiWindowFlags extra_flags)
		{
			return Internal_BeginChild(str_id, size_arg.X,size_arg.Y, border, (int)extra_flags);
		}

		public static void EndChild()
		{
			Internal_EndChild();
		}

		public static void End()
		{
			Internal_End();
		}


		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static void Internal_LabelText(string text);
		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static void Internal_Text(string text);
		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static bool Internal_Button(string text,float x,float y);
		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static bool Internal_Begin(string name, bool open, int flags);
		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static void Internal_End();
		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static bool Internal_BeginChild(string str_id, float size_arg_x, float size_arg_y, bool border, int extra_flags);
		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static void Internal_EndChild();
	}
}
