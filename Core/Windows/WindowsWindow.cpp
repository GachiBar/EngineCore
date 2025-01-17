// Copyright Epic Games, Inc. All Rights Reserved.

#include "WindowsWindow.h"

#include <dwmapi.h>
#include <algorithm>

#include "../Interfaces/IWindowsApplication.h"
#include "../libs/MathUtility.h"
#include "../libs/MemoryUtility.h"
#include "../Definitions.h"

#define INDEX_NONE (-1)

FWindowsWindow::~FWindowsWindow()
{
	// NOTE: The HWnd is invalid here!
	//       Doing stuff with HWnds will fail silently.
	//       Use Destroy() instead.
}

std::wstring FWindowsWindow::AppWindowClass = L"UnrealWindow";


std::shared_ptr< FWindowsWindow > FWindowsWindow::Make()
{
	// First, allocate the new native window object.  This doesn't actually create a native window or anything,
	// we're simply instantiating the object so that we can keep shared references to it.

	return std::shared_ptr<FWindowsWindow>(new FWindowsWindow());
}

void FWindowsWindow::Initialize(IWindowsApplication* const Application, const std::shared_ptr< FGenericWindowDefinition >& InDefinition, HINSTANCE InHInstance, const std::shared_ptr< FWindowsWindow >& InParent, const bool bShowImmediately )
{
	Definition = InDefinition;
	OwningApplication = Application;

	// Finally, let's initialize the new native window object.  Calling this function will often cause OS
	// window messages to be sent! (such as activation messages)
	uint32 window_ex_style = 0;
	uint32 WindowStyle = 0;

	RegionWidth = RegionHeight = INDEX_NONE;

	const float XInitialRect = Definition->XDesiredPositionOnScreen;
	const float YInitialRect = Definition->YDesiredPositionOnScreen;

	const float WidthInitial = Definition->WidthDesiredOnScreen;
	const float HeightInitial = Definition->HeightDesiredOnScreen;

	int32 ClientX =  XInitialRect;
	int32 ClientY = YInitialRect;
	int32 ClientWidth = WidthInitial;
	int32 ClientHeight = HeightInitial;
	int32 WindowX = ClientX;
	int32 WindowY = ClientY;
	int32 WindowWidth = ClientWidth;
	int32 WindowHeight = ClientHeight;
	


	if( !Definition->HasOSWindowBorder )
	{
		window_ex_style = WS_EX_WINDOWEDGE;

		if( Definition->TransparencySupport == EWindowTransparency::PerWindow )
		{
			window_ex_style |= WS_EX_LAYERED;
		}

		WindowStyle = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		if ( Definition->AppearsInTaskbar )
		{
			window_ex_style |= WS_EX_APPWINDOW;
		}
		else
		{
			window_ex_style |= WS_EX_TOOLWINDOW;
		}

		if( Definition->IsTopmostWindow )
		{
			// Tool tips are always top most windows
			window_ex_style |= WS_EX_TOPMOST;
		}

		if ( !Definition->AcceptsInput )
		{
			// Window should never get input
			window_ex_style |= WS_EX_TRANSPARENT;
		}
	}
	else
	{
		// OS Window border setup
		window_ex_style = WS_EX_APPWINDOW;
		WindowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;

		if (IsRegularWindow())
		{
			if (Definition->SupportsMaximize)
			{
				WindowStyle |= WS_MAXIMIZEBOX;
			}

			if (Definition->SupportsMinimize)
			{
				WindowStyle |= WS_MINIMIZEBOX;
			}

			if (Definition->HasSizingFrame)
			{
				WindowStyle |= WS_THICKFRAME;
			}
			else
			{
				WindowStyle |= WS_BORDER;
			}
		}
		else
		{
			WindowStyle |= WS_POPUP | WS_BORDER;
		}

		// X,Y, Width, Height defines the top-left pixel of the client area on the screen
		// This adjusts a zero rect to give us the size of the border
		RECT BorderRect = { 0, 0, 0, 0 };
		::AdjustWindowRectEx(&BorderRect, WindowStyle, false, window_ex_style);

		// Border rect size is negative - see MoveWindowTo
		WindowX += BorderRect.left;
		WindowY += BorderRect.top;

		// Inflate the window size by the OS border
		WindowWidth += BorderRect.right - BorderRect.left;
		WindowHeight += BorderRect.bottom - BorderRect.top;
	}

	RegisterWindowClass(InHInstance, AppWindowClass.c_str());

	// Creating the Window
	HWnd = CreateWindowEx(
		window_ex_style,
		AppWindowClass.c_str(),
		Definition->Title.c_str(),
		WindowStyle,
		WindowX, WindowY, 
		WindowWidth, WindowHeight,
		( InParent) ? InParent->HWnd : NULL,
		NULL, InHInstance, NULL);


	if (HWnd == NULL)
	{
		// @todo Error message should be localized!
		MessageBox(NULL, TEXT("Window Creation Failed!"), TEXT("Error!"), MB_ICONEXCLAMATION | MB_OK);

		const uint32 Error = GetLastError();

		// Get the number of handles.  A large number of windows has been known to cause window creation to fail because windows only allows so many.
		DWORD NumHandles = 0;
		GetProcessHandleCount(GetCurrentProcess(), &NumHandles);
		//TODO log this
        //checkf(0, TEXT("Window Creation Failed (%d). %d"), Error, NumHandles);

		return;
	}

	VirtualWidth = ClientWidth;
	VirtualHeight = ClientHeight;

	// We call reshape window here because we didn't take into account the non-client area
	// in the initial creation of the window. Slate should only pass client area dimensions.
	// Reshape window may resize the window if the non-client area is encroaching on our
	// desired client area space.
	ReshapeWindow( ClientX, ClientY, ClientWidth, ClientHeight );


	if ( Definition->TransparencySupport == EWindowTransparency::PerWindow )
	{
		SetOpacity( Definition->Opacity );
	}

	// Disable DWM Rendering and Nonclient Area painting if not showing the os window border
	// This prevents the standard windows frame from ever being drawn
	if( !Definition->HasOSWindowBorder )
	{
		const DWMNCRENDERINGPOLICY RenderingPolicy = DWMNCRP_DISABLED;
		//verify(SUCCEEDED(DwmSetWindowAttribute(HWnd, DWMWA_NCRENDERING_POLICY, &RenderingPolicy, sizeof(RenderingPolicy))));
        SUCCEEDED(DwmSetWindowAttribute(HWnd, DWMWA_NCRENDERING_POLICY, &RenderingPolicy, sizeof(RenderingPolicy)));
		const BOOL bEnableNCPaint = false;
		//verify(SUCCEEDED(DwmSetWindowAttribute(HWnd, DWMWA_ALLOW_NCPAINT, &bEnableNCPaint, sizeof(bEnableNCPaint))));
        SUCCEEDED(DwmSetWindowAttribute(HWnd, DWMWA_ALLOW_NCPAINT, &bEnableNCPaint, sizeof(bEnableNCPaint)));
	}

	// No region for non regular windows or windows displaying the os window border
	if ( IsRegularWindow() && !Definition->HasOSWindowBorder )
	{
		WindowStyle |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
		if ( Definition->SupportsMaximize )
		{
			WindowStyle |= WS_MAXIMIZEBOX;
		}
		if ( Definition->SupportsMinimize )
		{
			WindowStyle |= WS_MINIMIZEBOX;
		}
		if ( Definition->HasSizingFrame )
		{
			WindowStyle |= WS_THICKFRAME;
		}

		//verify(SetWindowLong(HWnd, GWL_STYLE, WindowStyle));
        SetWindowLong(HWnd, GWL_STYLE, WindowStyle);

		uint32 SetWindowPositionFlags = SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED;

		if ( Definition->ActivationPolicy == EWindowActivationPolicy::Never )
		{
			SetWindowPositionFlags |= SWP_NOACTIVATE;
		}

		::SetWindowPos(HWnd, nullptr, 0, 0, 0, 0, SetWindowPositionFlags);
		
		// For regular non-game windows delete the close menu from the default system menu. This prevents accidental closing of win32 apps by double clicking by accident on the application icon
		// The overwhelming majority of feedback is that is confusing behavior and we want to prevent this.
		::DeleteMenu(GetSystemMenu(HWnd, false), SC_CLOSE, MF_BYCOMMAND);

		AdjustWindowRegion( ClientWidth, ClientHeight );
	}
	else if (Definition->HasOSWindowBorder)
	{
		if (!Definition->HasCloseButton)
		{
			EnableMenuItem(GetSystemMenu(HWnd, false), SC_CLOSE, MF_GRAYED);
		}
	}
}

FWindowsWindow::FWindowsWindow()
	: HWnd(NULL)
	, WindowMode( EWindowMode::Windowed )
	, OLEReferenceCount(0)
	, AspectRatio(1.0f)
	, bIsVisible(false)
	, bIsFirstTimeVisible(true)
	, bInitiallyMinimized(false)
	, bInitiallyMaximized(false)
{
	// PreFullscreenWindowPlacement.length will be set when we save the window placement and then used to check if the structure is valid
	memset(&PreFullscreenWindowPlacement, 0, sizeof(PreFullscreenWindowPlacement));
   
    memset(&PreParentMinimizedWindowPlacement, 0, sizeof(PreParentMinimizedWindowPlacement));
   
	PreParentMinimizedWindowPlacement.length = sizeof(WINDOWPLACEMENT);
}

HWND FWindowsWindow::GetHWnd() const
{
	return HWnd;
}

HRGN FWindowsWindow::MakeWindowRegionObject(bool bIncludeBorderWhenMaximized) const
{
	HRGN Region;
	if ( RegionWidth != INDEX_NONE && RegionHeight != INDEX_NONE )
	{
		const bool bIsBorderlessGameWindow = GetDefinition().Type == EWindowType::GameWindow && !GetDefinition().HasOSWindowBorder;
		if (IsMaximizedWindow())
		{
			if (bIsBorderlessGameWindow)
			{
				// Windows caches the cxWindowBorders size at window creation. Even if borders are removed or resized Windows will continue to use this value when evaluating regions
				// and sizing windows. When maximized this means that our window position will be offset from the screen origin by (-cxWindowBorders,-cxWindowBorders). We want to
				// display only the region within the maximized screen area, so offset our upper left and lower right by cxWindowBorders.
				WINDOWINFO WindowInfo = {};

				WindowInfo.cbSize = sizeof(WindowInfo);
				::GetWindowInfo(HWnd, &WindowInfo);

				const int32 WindowBorderSize = bIncludeBorderWhenMaximized ? WindowInfo.cxWindowBorders : 0;
				Region = CreateRectRgn(WindowBorderSize, WindowBorderSize, RegionWidth + WindowBorderSize, RegionHeight + WindowBorderSize);
			}
			else
			{
				const int32 WindowBorderSize = bIncludeBorderWhenMaximized ? GetWindowBorderSize() : 0;
				Region = CreateRectRgn(WindowBorderSize, WindowBorderSize, RegionWidth - WindowBorderSize, RegionHeight - WindowBorderSize);
			}
		}
		else
		{
			const bool bUseCornerRadius  = WindowMode == EWindowMode::Windowed && !bIsBorderlessGameWindow &&
#if ALPHA_BLENDED_WINDOWS
				// Corner radii cause DWM window composition blending to fail, so we always set regions to full size rectangles
				Definition->TransparencySupport != EWindowTransparency::PerPixel &&
#endif
				Definition->CornerRadius > 0;

			if( bUseCornerRadius )
			{
				// CreateRoundRectRgn gives you a duff region that's 1 pixel smaller than you ask for. CreateRectRgn behaves correctly.
				// This can be verified by uncommenting the assert below
				Region = CreateRoundRectRgn( 0, 0, RegionWidth+1, RegionHeight+1, Definition->CornerRadius, Definition->CornerRadius );

				// Test that a point that should be in the region, is in the region
				// check(!!PtInRegion(Region, RegionWidth-1, RegionHeight/2));
			}
			else
			{
				Region = CreateRectRgn( 0, 0, RegionWidth, RegionHeight );
			}
		}
	}
	else
	{
		RECT rcWnd;
		GetWindowRect( HWnd, &rcWnd );
		Region = CreateRectRgn( 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top );
	}

	return Region;
}

void FWindowsWindow::RegisterWindowClass(HINSTANCE instance, LPCWSTR window_name)
{
	WNDCLASSEX window_class;
	window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	window_class.lpfnWndProc = OwningApplication->GetWndProc();
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0;
	window_class.hInstance = instance;
	window_class.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	window_class.hIconSm = window_class.hIcon;
	window_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
	window_class.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	window_class.lpszMenuName = nullptr;
	window_class.lpszClassName = window_name;
	window_class.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&window_class);
}

void FWindowsWindow::AdjustWindowRegion( int32 Width, int32 Height )
{
	RegionWidth = Width;
	RegionHeight = Height;

	HRGN Region = MakeWindowRegionObject(true);

	// NOTE: We explicitly don't delete the Region object, because the OS deletes the handle when it no longer needed after
	// a call to SetWindowRgn.

	//verify( SetWindowRgn( HWnd, Region, false ) );
    SetWindowRgn( HWnd, Region, false );
}

void FWindowsWindow::ReshapeWindow( int32 NewX, int32 NewY, int32 NewWidth, int32 NewHeight )
{
	WINDOWINFO WindowInfo;
	FMemory::Memzero( WindowInfo );
	WindowInfo.cbSize = sizeof( WindowInfo );
	::GetWindowInfo( HWnd, &WindowInfo );

	AspectRatio = (float)NewWidth / (float)NewHeight;

	// X,Y, Width, Height defines the top-left pixel of the client area on the screen
	if( Definition->HasOSWindowBorder )
	{
		// This adjusts a zero rect to give us the size of the border
		RECT BorderRect = { 0, 0, 0, 0 };
		::AdjustWindowRectEx(&BorderRect, WindowInfo.dwStyle, false, WindowInfo.dwExStyle);

		// Border rect size is negative - see MoveWindowTo
		NewX += BorderRect.left;
		NewY += BorderRect.top;

		// Inflate the window size by the OS border
		NewWidth += BorderRect.right - BorderRect.left;
		NewHeight += BorderRect.bottom - BorderRect.top;
	}

	// the window position is the requested position
	int32 WindowX = NewX;
	int32 WindowY = NewY;
	
	// If the window size changes often, only grow the window, never shrink it
	const bool bVirtualSizeChanged = NewWidth != VirtualWidth || NewHeight != VirtualHeight;
	VirtualWidth = NewWidth;
	VirtualHeight = NewHeight;

	if( Definition->SizeWillChangeOften )
	{
		// When SizeWillChangeOften is set, we set a minimum width and height window size that we'll keep allocated
		// even when the requested actual window size is smaller.  This just avoids constantly resizing the window
		// and associated GPU buffers, which can be very slow on some platforms.

		const RECT OldWindowRect = WindowInfo.rcWindow;
		const int32 OldWidth = OldWindowRect.right - OldWindowRect.left;
		const int32 OldHeight = OldWindowRect.bottom - OldWindowRect.top;

		const int32 MinRetainedWidth = Definition->ExpectedMaxWidth != INDEX_NONE ? Definition->ExpectedMaxWidth : OldWidth;
		const int32 MinRetainedHeight = Definition->ExpectedMaxHeight != INDEX_NONE ? Definition->ExpectedMaxHeight : OldHeight;

		NewWidth = FMath::Max( NewWidth, FMath::Min( OldWidth, MinRetainedWidth) ) ;
		NewHeight = FMath::Max( NewHeight, FMath::Min( OldHeight, MinRetainedHeight ) );
	}

	if (IsMaximizedWindow())
	{
		Restore();
	}

	// We use SWP_NOSENDCHANGING when in fullscreen mode to prevent Windows limiting our window size to the current resolution, as that 
	// prevents us being able to change to a higher resolution while in fullscreen mode
	::SetWindowPos( HWnd, nullptr, WindowX, WindowY, NewWidth, NewHeight, SWP_NOZORDER | SWP_NOACTIVATE | ((WindowMode == EWindowMode::Fullscreen) ? SWP_NOSENDCHANGING : 0) );
	
	bool bAdjustSizeChange = Definition->SizeWillChangeOften || bVirtualSizeChanged;
	bool bAdjustCorners = Definition->Type != EWindowType::Menu && Definition->CornerRadius > 0;

	if (!Definition->HasOSWindowBorder && (bAdjustSizeChange || bAdjustCorners))
	{
		AdjustWindowRegion( VirtualWidth, VirtualHeight );
	}
}

bool FWindowsWindow::GetFullScreenInfo( int32& X, int32& Y, int32& Width, int32& Height ) const
{
	bool bTrueFullscreen = ( WindowMode == EWindowMode::Fullscreen );

	// Grab current monitor data for sizing
	HMONITOR Monitor = MonitorFromWindow( HWnd, bTrueFullscreen ? MONITOR_DEFAULTTOPRIMARY : MONITOR_DEFAULTTONEAREST );
	MONITORINFO MonitorInfo;
	MonitorInfo.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo( Monitor, &MonitorInfo );

	X = MonitorInfo.rcMonitor.left;
	Y = MonitorInfo.rcMonitor.top;
	Width = MonitorInfo.rcMonitor.right - X;
	Height = MonitorInfo.rcMonitor.bottom - Y;

	return true;
}

/** Native windows should implement MoveWindowTo by relocating the client area of the platform-specific window to (X,Y). */
void FWindowsWindow::MoveWindowTo( int32 X, int32 Y )
{
	// Slate gives the window position as relative to the client area of a window, so we may need to compensate for the OS border
	if (Definition->HasOSWindowBorder)
	{
		const LONG WindowStyle = ::GetWindowLong(HWnd, GWL_STYLE);
		const LONG WindowExStyle = ::GetWindowLong(HWnd, GWL_EXSTYLE);

		// This adjusts a zero rect to give us the size of the border
		RECT BorderRect = { 0, 0, 0, 0 };
		::AdjustWindowRectEx(&BorderRect, WindowStyle, false, WindowExStyle);

		// Border rect size is negative
		X += BorderRect.left;
		Y += BorderRect.top;
	}

	::SetWindowPos(HWnd, nullptr, X, Y, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER);
}

/** Native windows should implement BringToFront by making this window the top-most window (i.e. focused).
 *
 * @param bForce	Forces the window to the top of the Z order, even if that means stealing focus from other windows
 *					In general do not pass true for this.  It is really only useful for some windows, like game windows where not forcing it to the front
 *					could cause mouse capture and mouse lock to happen but without the window visible
 */
void FWindowsWindow::BringToFront( bool bForce )
{
	if ( IsRegularWindow() )
	{
		if (::IsIconic(HWnd))
		{
			::ShowWindow(HWnd, SW_RESTORE);
		}
		else
		{
			::SetActiveWindow(HWnd);
		}
	}
	else
	{
		HWND HWndInsertAfter = HWND_TOP;
		// By default we activate the window or it isn't actually brought to the front 
		uint32 Flags = SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER;

		if( !bForce )
		{
			Flags |= SWP_NOACTIVATE;
		}

		if( Definition->IsTopmostWindow )
		{
			HWndInsertAfter = HWND_TOPMOST;
		}

		::SetWindowPos( HWnd, HWndInsertAfter, 0, 0, 0, 0, Flags );
	}
}

void FWindowsWindow::HACK_ForceToFront()
{
	::SetForegroundWindow(HWnd);
}

/** Native windows should implement this function by asking the OS to destroy OS-specific resource associated with the window (e.g. Win32 window handle) */
void FWindowsWindow::Destroy()
{
	if (OLEReferenceCount > 0 && IsWindow(HWnd))
	{
		HRESULT Result = RevokeDragDrop(HWnd);
		// If we decremented OLEReferenceCount check it for being null (shutdown)
		if (Result == S_OK)
		{
			//ensureMsgf(OLEReferenceCount == 0, TEXT("Not all references to window are released, %i left"), OLEReferenceCount);
		}
	}

	::DestroyWindow( HWnd );
}

/** Native window should implement this function by performing the equivalent of the Win32 minimize-to-taskbar operation */
void FWindowsWindow::Minimize()
{
	// Windows window handles the initial show state on the first Show call in order to handle activation policy.
	// So we only call now if that already happened.
	if (!bIsFirstTimeVisible)
	{
		::ShowWindow(HWnd, SW_MINIMIZE);
	}
	else
	{
		bInitiallyMinimized = true;
		bInitiallyMaximized = false;
	}
}

/** Native window should implement this function by performing the equivalent of the Win32 maximize operation */
void FWindowsWindow::Maximize()
{
	// Windows window handles the initial show state on the first Show call in order to handle activation policy.
	// So we only call now if that already happened.
	if (!bIsFirstTimeVisible)
	{
		::ShowWindow(HWnd, SW_MAXIMIZE);
	}
	else
	{
		bInitiallyMaximized = true;
		bInitiallyMinimized = false;
	}
}

/** Native window should implement this function by performing the equivalent of the Win32 maximize operation */
void FWindowsWindow::Restore()
{
	// Windows window handles the initial show state on the first Show call in order to handle activation policy.
	// So we only call now if that already happened.
	if (!bIsFirstTimeVisible)
	{
		::ShowWindow(HWnd, SW_RESTORE);
	}
	else
	{
		bInitiallyMaximized = false;
		bInitiallyMinimized = false;
	}
}

/** Native window should make itself visible */
void FWindowsWindow::Show()
{
	if (!bIsVisible)
	{
		bIsVisible = true;

		// Should the show command include activation?
		// Do not activate windows that do not take input; e.g. tool-tips and cursor decorators
		bool bShouldActivate = false;
		if (Definition->AcceptsInput)
		{
			bShouldActivate = Definition->ActivationPolicy == EWindowActivationPolicy::Always;
			if (bIsFirstTimeVisible && Definition->ActivationPolicy == EWindowActivationPolicy::FirstShown)
			{
				bShouldActivate = true;
			}
		}

		// Map to the relevant ShowWindow command.
		int ShowWindowCommand = bShouldActivate ? SW_SHOW : SW_SHOWNOACTIVATE;
		if (bIsFirstTimeVisible)
		{
			bIsFirstTimeVisible = false;
			if (bInitiallyMinimized)
			{
				ShowWindowCommand = bShouldActivate ? SW_MINIMIZE : SW_SHOWMINNOACTIVE;
			}
			else if (bInitiallyMaximized)
			{
				ShowWindowCommand = bShouldActivate ? SW_SHOWMAXIMIZED : SW_MAXIMIZE;
			}
		}

		::ShowWindow(HWnd, ShowWindowCommand);

		// Turns out SW_SHOWNA doesn't work correctly if the window has never been shown before.  If the window
		// was already maximized, (and hidden) and we're showing it again, SW_SHOWNA would be right.  But it's not right
		// to use SW_SHOWNA when the window has never been shown before!
		// 
		// TODO Add in a more complicated path that involves SW_SHOWNA if we hide windows in their maximized/minimized state.
		//::ShowWindow(HWnd, bShouldActivate ? SW_SHOW : SW_SHOWNA);
	}
}

/** Native window should hide itself */
void FWindowsWindow::Hide()
{
	if (bIsVisible)
	{
		bIsVisible = false;
		::ShowWindow(HWnd, SW_HIDE);
	}
}

/** Toggle native window between fullscreen and normal mode */
void FWindowsWindow::SetWindowMode( EWindowMode::Type NewWindowMode )
{
	if (NewWindowMode != WindowMode)
	{
		EWindowMode::Type PreviousWindowMode = WindowMode;
		WindowMode = NewWindowMode;

		const bool bTrueFullscreen = NewWindowMode == EWindowMode::Fullscreen;

		// Setup Win32 Flags to be used for Fullscreen mode
		LONG WindowStyle = GetWindowLong(HWnd, GWL_STYLE);
		const LONG FullscreenModeStyle = WS_POPUP;

		LONG WindowedModeStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
		if (IsRegularWindow())
		{
			if (Definition->SupportsMaximize)
			{
				WindowedModeStyle |= WS_MAXIMIZEBOX;
			}

			if (Definition->SupportsMinimize)
			{
				WindowedModeStyle |= WS_MINIMIZEBOX;
			}

			if (Definition->HasSizingFrame)
			{
				WindowedModeStyle |= WS_THICKFRAME;
			}
			else
			{
				WindowedModeStyle |= WS_BORDER;
			}
		}
		else
		{
			WindowedModeStyle |= WS_POPUP | WS_BORDER;
		}

		// If we're not in fullscreen, make it so
		if (NewWindowMode == EWindowMode::WindowedFullscreen || NewWindowMode == EWindowMode::Fullscreen)
		{
			if (PreviousWindowMode == EWindowMode::Windowed)
			{
				PreFullscreenWindowPlacement.length = sizeof(WINDOWPLACEMENT);
				::GetWindowPlacement(HWnd, &PreFullscreenWindowPlacement);
			}

			// Setup Win32 flags for fullscreen window
			WindowStyle &= ~WindowedModeStyle;
			WindowStyle |= FullscreenModeStyle;

			SetWindowLong(HWnd, GWL_STYLE, WindowStyle);
			::SetWindowPos(HWnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

			if (!bTrueFullscreen)
			{
				// Ensure the window is restored if we are going for WindowedFullscreen
				ShowWindow(HWnd, SW_RESTORE);
			}

			// Get the current window position.
			RECT ClientRect;
			GetClientRect(HWnd, &ClientRect);

			// Grab current monitor data for sizing
			HMONITOR Monitor = MonitorFromWindow( HWnd, bTrueFullscreen ? MONITOR_DEFAULTTOPRIMARY : MONITOR_DEFAULTTONEAREST );
			MONITORINFO MonitorInfo;
			MonitorInfo.cbSize = sizeof(MONITORINFO);
			GetMonitorInfo( Monitor, &MonitorInfo );

			// Get the target client width to send to ReshapeWindow.
			// Preserve the current res if going to true fullscreen and the monitor supports it and allow the calling code
			// to resize if required.
			// Else, use the monitor's res for windowed fullscreen.
			LONG MonitorWidth  = MonitorInfo.rcMonitor.right - MonitorInfo.rcMonitor.left;
			LONG TargetClientWidth = bTrueFullscreen ?
				FMath::Min(MonitorWidth, ClientRect.right - ClientRect.left) :
				MonitorWidth;

			LONG MonitorHeight = MonitorInfo.rcMonitor.bottom - MonitorInfo.rcMonitor.top;
			LONG TargetClientHeight = bTrueFullscreen ?
				FMath::Min(MonitorHeight, ClientRect.bottom - ClientRect.top) :
				MonitorHeight;


			// Resize and position fullscreen window
			ReshapeWindow(
				MonitorInfo.rcMonitor.left,
				MonitorInfo.rcMonitor.top,
				TargetClientWidth,
				TargetClientHeight);
		}
		else
		{
			// Windowed:

			// Setup Win32 flags for restored window
			WindowStyle &= ~FullscreenModeStyle;
			WindowStyle |= WindowedModeStyle;
			SetWindowLong(HWnd, GWL_STYLE, WindowStyle);
			::SetWindowPos(HWnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

			if (PreFullscreenWindowPlacement.length) // Was PreFullscreenWindowPlacement initialized?
			{
				::SetWindowPlacement(HWnd, &PreFullscreenWindowPlacement);
			}

			// Set the icon back again as it seems to get ignored if the application has ever started in full screen mode
			HICON HIcon = (HICON)::GetClassLongPtr( HWnd, GCLP_HICON );
			if (HIcon != nullptr)
			{
				::SendMessageW( HWnd, WM_SETICON, ICON_SMALL, (LPARAM)HIcon );
			}
		}
	}
}

/** @return true if the native window is maximized, false otherwise */
bool FWindowsWindow::IsMaximizedWindow() const
{
	bool bIsMaximized = IsZoomed(HWnd);
	return bIsMaximized;
}

bool FWindowsWindow::IsMinimizedWindow() const
{
	return IsIconic(HWnd);
}

/** @return true if the native window is visible, false otherwise */
bool FWindowsWindow::IsVisible() const
{
	return bIsVisible;
}

/** Returns the size and location of the window when it is restored */
bool FWindowsWindow::GetRestoredDimensions(int32& X, int32& Y, int32& Width, int32& Height)
{
	WINDOWPLACEMENT WindowPlacement;
	WindowPlacement.length = sizeof( WINDOWPLACEMENT );

	if( ::GetWindowPlacement( HWnd, &WindowPlacement) != 0 )
	{
		// This window may be minimized.  Get the position when it is restored.
		const RECT Restored = WindowPlacement.rcNormalPosition;

		X = Restored.left;
		Y = Restored.top;
		Width = Restored.right - Restored.left;
		Height = Restored.bottom - Restored.top;

		const LONG WindowExStyle = ::GetWindowLong(HWnd, GWL_EXSTYLE);
		if ((WindowExStyle & WS_EX_TOOLWINDOW) == 0)
		{
			// For windows without WS_EX_TOOLWINDOW window style WindowPlacement.rcNormalPosition is in workspace coordinates, so we need to convert the position to screen coordinates
			const bool bTrueFullscreen = (WindowMode == EWindowMode::Fullscreen);
			HMONITOR Monitor = MonitorFromWindow(HWnd, bTrueFullscreen ? MONITOR_DEFAULTTOPRIMARY : MONITOR_DEFAULTTONEAREST);
			MONITORINFO MonitorInfo;
			MonitorInfo.cbSize = sizeof(MONITORINFO);
			GetMonitorInfo(Monitor, &MonitorInfo);

			X += MonitorInfo.rcWork.left - MonitorInfo.rcMonitor.left;
			Y += MonitorInfo.rcWork.top - MonitorInfo.rcMonitor.top;
		}

		return true;
	}
	else
	{
		return false;
	}
}

void FWindowsWindow::OnParentWindowMinimized()
{
	// This function is called from SW_PARENTCLOSING, because there's a bug in Win32 that causes the equivalent SW_PARENTOPENING
	// message to restore in an incorrect state (eg, it will lose the maximized status of the window)
	// To work around this, we cache our window placement here so that we can restore it later (see OnParentWindowRestored)
	::GetWindowPlacement(HWnd, &PreParentMinimizedWindowPlacement);
}

void FWindowsWindow::OnParentWindowRestored()
{
	// This function is called from SW_PARENTOPENING so that we can restore the window placement that was cached in OnParentWindowMinimized
	::SetWindowPlacement(HWnd, &PreParentMinimizedWindowPlacement);
}

/** Sets focus on the native window */
void FWindowsWindow::SetWindowFocus()
{
	if( GetFocus() != HWnd )
	{
		::SetFocus( HWnd );
	}
}

/**
 * Sets the opacity of this window
 *
 * @param	InOpacity	The new window opacity represented as a floating point scalar
 */
void FWindowsWindow::SetOpacity( const float InOpacity )
{
	SetLayeredWindowAttributes( HWnd, 0, BYTE( InOpacity * 255.0f ), LWA_ALPHA );
}

/**
 * Enables or disables this window.  When disabled, a window will receive no input.       
 *
 * @param bEnable	true to enable the window, false to disable it.
 */
void FWindowsWindow::Enable( bool bEnable )
{
	::EnableWindow( HWnd, bEnable );
}

/** @return True if the window is enabled */
bool FWindowsWindow::IsEnabled()
{
	return !!::IsWindowEnabled( HWnd );
}

/** @return true if native window exists underneath the coordinates */
bool FWindowsWindow::IsPointInWindow( int32 X, int32 Y ) const
{
	bool Result = false;

	HRGN Region = MakeWindowRegionObject(false);
	Result = !!PtInRegion( Region, X, Y );
	DeleteObject( Region );

	return Result;
}

int32 FWindowsWindow::GetWindowBorderSize() const
{
	if (GetDefinition().Type == EWindowType::GameWindow && !GetDefinition().HasOSWindowBorder)
	{
		// Our borderless game windows actually have a thick border to allow sizing, which we draw over to simulate
		// a borderless window. We return zero here so that the game will correctly behave as if this is truly a
		// borderless window.
		return 0;
	}

	WINDOWINFO WindowInfo;
	FMemory::Memzero( WindowInfo );
	WindowInfo.cbSize = sizeof( WindowInfo );
	::GetWindowInfo( HWnd, &WindowInfo );

	return WindowInfo.cxWindowBorders;
}

int32 FWindowsWindow::GetWindowTitleBarSize() const
{
	return GetSystemMetrics(SM_CYCAPTION);
}

bool FWindowsWindow::IsForegroundWindow() const
{
	return ::GetForegroundWindow() == HWnd;
}

bool FWindowsWindow::IsFullscreenSupported() const
{
	// fullscreen not supported when using remote desktop
	return !::GetSystemMetrics(SM_REMOTESESSION);
}

void FWindowsWindow::SetText( std::wstring const & Text )
{
	SetWindowText(HWnd, Text.c_str());
}

IApplication* FWindowsWindow::GetOwningApplication()
{
	return OwningApplication;
}

bool FWindowsWindow::IsRegularWindow() const
{
	return Definition->IsRegularWindow;
}
