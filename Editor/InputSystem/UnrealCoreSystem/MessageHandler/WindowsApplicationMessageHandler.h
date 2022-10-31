#pragma once

#include <set>
#include "GenericApplicationMessageHandler.h"
#include "../KeyStruct.h"

struct WindowsApplicationMessageHandler : public FGenericApplicationMessageHandler
{
	virtual bool OnKeyChar( const char Character, const bool IsRepeat ) override;
	virtual bool OnKeyDown( const int32 KeyCode, const uint32 CharacterCode, const bool IsRepeat ) override;
	virtual bool OnKeyUp( const int32 KeyCode, const uint32 CharacterCode, const bool IsRepeat ) override;
	virtual void OnInputLanguageChanged() override;
	virtual bool OnMouseDown( const std::shared_ptr< FGenericWindow >& PlatformWindow, const EMouseButtons::Type Button ) override;
	virtual bool OnMouseDown( const std::shared_ptr< FGenericWindow >& PlatformWindow, const EMouseButtons::Type Button, const FVector2D CursorPos ) override;
	virtual bool OnMouseUp( const EMouseButtons::Type Button ) override;
	virtual bool OnMouseUp( const EMouseButtons::Type Button, const FVector2D CursorPos ) override;
	virtual bool OnMouseDoubleClick( const std::shared_ptr< FGenericWindow >& PlatformWindow, const EMouseButtons::Type Button ) override;
	virtual bool OnMouseDoubleClick( const std::shared_ptr< FGenericWindow >& PlatformWindow, const EMouseButtons::Type Button, const FVector2D CursorPos ) override;
	virtual bool OnMouseWheel( const float Delta ) override;
	virtual bool OnMouseWheel( const float Delta, const FVector2D CursorPos ) override;
	virtual bool OnMouseMove() override;
	virtual void OnSizeChanged(const std::shared_ptr< FGenericWindow >& PlatformWindow, const int32 Width, const int32 Height, bool bWasMinimized) override;
	virtual void OnResizingWindow( const std::shared_ptr< FGenericWindow >& PlatformWindow ) override;
	virtual void OnMovedWindow( const std::shared_ptr< FGenericWindow >& PlatformWindow, const int32 X, const int32 Y ) override;
	virtual bool OnApplicationActivationChanged( const bool InAppActivated) override;
	virtual void OnWindowClose( const std::shared_ptr< FGenericWindow >& PlatformWindow ) override;
    virtual void SetCursorPos( const FVector2D& MouseCoordinate ) override;
protected:
	/** true when the slate app is active; i.e. the current foreground window is from our Slate app*/
	bool bAppIsActive = false;

  
};