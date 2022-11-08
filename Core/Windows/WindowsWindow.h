#pragma once
#include <memory>
#include <windows.h>
#include "GenericWindow.h"

class IWindowsApplication;

/**
 * A platform specific implementation of FGenericWindow.
 *
 */
class FWindowsWindow
	: public FGenericWindow
{
public:

	/** Win32 requirement: see CreateWindowEx and RegisterClassEx. */
	static std::wstring AppWindowClass;

public:

	/** Destructor. */
	~FWindowsWindow() override;

	/** Create a new FWin32Window. */
	static std::shared_ptr<FWindowsWindow> Make();

	/**
	 * Gets the Window's handle.
	 *
	 * @return The window's HWND handle.
	 */
	HWND GetHWnd() const;

	void Initialize(IWindowsApplication* const Application, const std::shared_ptr<FGenericWindowDefinition>& InDefinition, HINSTANCE InHInstance, const std::shared_ptr<FWindowsWindow>& InParent, const bool bShowImmediately );

	bool IsRegularWindow() const;

	/**
     * Sets the window region to specified dimensions.
	 *
	 * @param Width The width of the window region (in pixels).
	 * @param Height The height of the window region (in pixels).
	 */
	void AdjustWindowRegion(int32 Width, int32 Height);

	/** Called when our parent window is minimized (which will in turn cause us to become minimized). */
	void OnParentWindowMinimized();

	/** Called when our parent window is restored (which will in turn cause us to become restored). */
	void OnParentWindowRestored();

	float GetAspectRatio() const { return AspectRatio; }

	/** @return True if the window is enabled */
	bool IsEnabled();

public:

	// FGenericWindow interface

	virtual void ReshapeWindow( int32 X, int32 Y, int32 Width, int32 Height ) override;
	virtual bool GetFullScreenInfo( int32& X, int32& Y, int32& Width, int32& Height ) const override;
	virtual void MoveWindowTo ( int32 X, int32 Y ) override;
	virtual void BringToFront( bool bForce = false ) override;
	virtual void HACK_ForceToFront() override;
	virtual void Destroy() override;
	virtual void Minimize() override;
	virtual void Maximize() override;
	virtual void Restore() override;
	virtual void Show() override;
	virtual void Hide() override;
	virtual void SetWindowMode( EWindowMode::Type NewWindowMode ) override;
	virtual EWindowMode::Type GetWindowMode() const override { return WindowMode; }


	virtual bool IsMaximizedWindow() const override;
	virtual bool IsMinimizedWindow() const override;

	virtual bool IsVisible() const override;
	virtual bool GetRestoredDimensions(int32& X, int32& Y, int32& Width, int32& Height) override;
	virtual void SetWindowFocus() override;
	virtual void SetOpacity( const float InOpacity ) override;
	virtual void Enable( bool bEnable ) override;
	virtual bool IsPointInWindow( int32 X, int32 Y ) const override;
	virtual int32 GetWindowBorderSize() const override;
	virtual int32 GetWindowTitleBarSize() const override;
	virtual void* GetOSWindowHandle() const  override { return HWnd; }
	virtual bool IsForegroundWindow() const override;
	virtual bool IsFullscreenSupported() const override;
	virtual void SetText(std::wstring const & Text) override;
	IApplication* GetOwningApplication() override;

private:

	/** Protect the constructor; only TSharedRefs of this class can be made. */
	FWindowsWindow();

	/** Creates an HRGN for the window's current region.  Remember to delete this when you're done with it using
	   ::DeleteObject, unless you're passing it to SetWindowRgn(), which will absorb the reference itself. */
	HRGN MakeWindowRegionObject(bool bIncludeBorderWhenMaximized) const;


	void RegisterWindowClass(HINSTANCE instance, LPCWSTR window_name);

private:

	/** The application that owns this window. */
	IWindowsApplication* OwningApplication;

	/** The window's handle. */
	HWND HWnd;

	/** Store the window region size for querying whether a point lies within the window. */
	int32 RegionWidth;
	int32 RegionHeight;
	
	/** The mode that the window is in (windowed, fullscreen, windowedfullscreen ) */
	EWindowMode::Type WindowMode;

	/** This object's reference count (for the IUnknown interface). */
	int32 OLEReferenceCount;
		
	/** The placement of the window before it entered a fullscreen state. */
	WINDOWPLACEMENT PreFullscreenWindowPlacement;

	/** The placement of the window before it entered a minimized state due to its parent window being minimized. */
	WINDOWPLACEMENT PreParentMinimizedWindowPlacement;

	/** Virtual width and height of the window.  This is only different than the actual width and height for
	    windows which we're trying to optimize because their size changes frequently.  We'll create a larger
		window and have Windows draw it "cropped" so that it appears smaller, rather than actually resizing
		it and incurring a GPU buffer resize performance hit */
	int32 VirtualWidth;
	int32 VirtualHeight;

	/** Current aspect ratio of window's client area */
	float AspectRatio;

	/** Whether the window is currently shown */
	bool bIsVisible : 1;

	/** Whether the window is yet to have its first Show() call. This is set false after first Show(). */
	bool bIsFirstTimeVisible : 1;

	/** We cache the min/max state for any Minimize, Maximize, or Restore calls that were made before the first Show */
	bool bInitiallyMinimized : 1;
	bool bInitiallyMaximized : 1;

	/** when true the window is responsible for its own size adjustments in response to a DPI change */
	bool bHandleManualDPIChanges = false;
};
