#include "GenericWindow.h"

FGenericWindow::FGenericWindow()
{

}

FGenericWindow::~FGenericWindow()
{

}

void FGenericWindow::ReshapeWindow( int32 X, int32 Y, int32 Width, int32 Height )
{
	// empty default functionality
}

bool FGenericWindow::GetFullScreenInfo( int32& X, int32& Y, int32& Width, int32& Height ) const
{
	// this function cannot return valid results, and should not be needed on consoles, etc
	//UE_LOG(LogGenericPlatformWindow, Fatal, TEXT("GetFullScreenInfo is not expected to be called on this platform"));
	return false;
}

void FGenericWindow::MoveWindowTo ( int32 X, int32 Y )
{
	// empty default functionality
}

void FGenericWindow::BringToFront( bool bForce )
{
	// empty default functionality
}

void FGenericWindow::HACK_ForceToFront()
{
	// empty default functionality
}

void FGenericWindow::Destroy()
{
	// empty default functionality
}

void FGenericWindow::Minimize()
{
	// empty default functionality
}

void FGenericWindow::Maximize()
{
	// empty default functionality
}

void FGenericWindow::Restore()
{
	// empty default functionality
}

void FGenericWindow::Show()
{
	// empty default functionality
}

void FGenericWindow::Hide()
{
	// empty default functionality
}

void FGenericWindow::SetWindowMode( EWindowMode::Type InNewWindowMode )
{
	// empty default functionality
}

EWindowMode::Type FGenericWindow::GetWindowMode() const
{
	// default functionality
	return EWindowMode::Windowed;
}

bool FGenericWindow::IsMaximizedWindow() const
{
	// empty default functionality
	return true;
}

bool FGenericWindow::IsMinimizedWindow() const
{
	return false;
}


bool FGenericWindow::IsVisible() const
{
	// empty default functionality
	return true;
}

bool FGenericWindow::GetRestoredDimensions(int32& X, int32& Y, int32& Width, int32& Height)
{
	// this function cannot return valid results, and should not be needed on consoles, etc
	//UE_LOG(LogGenericPlatformWindow, Fatal, TEXT("GetRestoredDimensions is not expected to be called on this platform"));
	return false;
}

void FGenericWindow::SetWindowFocus()
{
	// empty default functionality
}

void FGenericWindow::SetOpacity( const float InOpacity )
{
	// empty default functionality
}

void FGenericWindow::Enable( bool bEnable )
{
	// empty default functionality
}

bool FGenericWindow::IsPointInWindow( int32 X, int32 Y ) const
{
	// empty default functionality
	return true;
}
	
int32 FGenericWindow::GetWindowBorderSize() const
{
	// empty default functionality
	return 0;
}

int32 FGenericWindow::GetWindowTitleBarSize() const
{
	// empty default functionality
	return 0;
}

void* FGenericWindow::GetOSWindowHandle() const
{
	return nullptr;
}

bool FGenericWindow::IsForegroundWindow() const 
{
	// empty default functionality
	return true;
}

bool FGenericWindow::IsFullscreenSupported() const
{
	// empty default functionality
	return true;
}

void FGenericWindow::SetText(std::wstring const& Text)
{
	// empty default functionality
}

const FGenericWindowDefinition& FGenericWindow::GetDefinition() const
{
	return *Definition;
}

bool FGenericWindow::IsDefinitionValid() const
{
	return Definition.get();
}



void FGenericWindow::SetNativeWindowButtonsVisibility(bool bVisible)
{
	// empty default functionality
}
