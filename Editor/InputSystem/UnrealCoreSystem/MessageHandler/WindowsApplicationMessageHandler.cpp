#include "WindowsApplicationMessageHandler.h"
#include "Windows.h"
#include "../InputEvent/Events.h"
#include "../KeyStruct.h"
#include "../Keys.h"
#include "../InputKeyManager.h"

static FKey TranslateMouseButtonToKey( const EMouseButtons::Type Button )
{
	FKey Key = EKeys::Invalid;

	switch( Button )
	{
	case EMouseButtons::Left:
		Key = EKeys::LeftMouseButton;
		break;
	case EMouseButtons::Middle:
		Key = EKeys::MiddleMouseButton;
		break;
	case EMouseButtons::Right:
		Key = EKeys::RightMouseButton;
		break;
	case EMouseButtons::Thumb01:
		Key = EKeys::ThumbMouseButton;
		break;
	case EMouseButtons::Thumb02:
		Key = EKeys::ThumbMouseButton2;
		break;
	}

	return Key;
}

bool WindowsApplicationMessageHandler::OnKeyChar( const char Character, const bool IsRepeat )
{
	FCharacterEvent CharacterEvent( Character, IsRepeat );
	return ProcessKeyCharEvent( CharacterEvent );
}

bool WindowsApplicationMessageHandler::OnKeyDown(const int32 KeyCode, const uint32 CharacterCode, const bool IsRepeat)
{
	FKey const Key = InputKeyManager::Get().GetKeyFromCodes(KeyCode, CharacterCode);
	FKeyEvent KeyEvent(Key, IsRepeat, CharacterCode, KeyCode);

	return ProcessKeyDownEvent(KeyEvent);
}

bool WindowsApplicationMessageHandler::OnKeyUp( const int32 KeyCode, const uint32 CharacterCode, const bool IsRepeat )
{
	FKey const Key = InputKeyManager::Get().GetKeyFromCodes( KeyCode, CharacterCode );
	FKeyEvent KeyEvent(Key, IsRepeat, CharacterCode, KeyCode);

	return ProcessKeyUpEvent( KeyEvent );
}

void WindowsApplicationMessageHandler::OnInputLanguageChanged()
{
	InputKeyManager::Get().InitKeyMappings();
}

bool WindowsApplicationMessageHandler::OnMouseDown(const std::shared_ptr< FGenericWindow >& PlatformWindow, const EMouseButtons::Type Button)
{
    //TODO move it to some class
    POINT p;
    GetCursorPos(&p);
	
	return OnMouseDown(PlatformWindow,Button, FVector2D(p.x, p.y));
}

bool WindowsApplicationMessageHandler::OnMouseDown(const std::shared_ptr<FGenericWindow>& PlatformWindow,
	const EMouseButtons::Type Button, const FVector2D CursorPos)
{

	FKey Key = TranslateMouseButtonToKey(Button);

	FPointerEvent MouseEvent(
		CursorPos,
		GetLastCursorPos(),
		PressedMouseButtons,
		Key,
		0.f
	);

	PressedMouseButtons.insert(MouseEvent.GetEffectingButton());
	return ProcessMouseButtonDownEvent(PlatformWindow, MouseEvent);
}

bool WindowsApplicationMessageHandler::OnMouseDoubleClick( const std::shared_ptr< FGenericWindow >& PlatformWindow, const EMouseButtons::Type Button )
{
    POINT p;
    GetCursorPos(&p);
	return OnMouseDoubleClick(PlatformWindow, Button, p);
}

bool WindowsApplicationMessageHandler::OnMouseDoubleClick( const std::shared_ptr< FGenericWindow >& PlatformWindow, const EMouseButtons::Type Button, const FVector2D CursorPos )
{

	FKey Key = TranslateMouseButtonToKey( Button );

	FPointerEvent MouseEvent(
		CursorPos,
		GetLastCursorPos(),
		PressedMouseButtons,
		Key,
		0
		);

    PressedMouseButtons.Add( InMouseEvent.GetEffectingButton() );

	return ProcessMouseButtonDoubleClickEvent( PlatformWindow, MouseEvent );
}

bool WindowsApplicationMessageHandler::OnMouseUp( const EMouseButtons::Type Button )
{
    POINT p;
    GetCursorPos(&p);
	return OnMouseUp(Button, p);
}

bool WindowsApplicationMessageHandler::OnMouseUp( const EMouseButtons::Type Button, const FVector2D CursorPos )
{
	FKey Key = TranslateMouseButtonToKey( Button );

	FPointerEvent MouseEvent(
		CursorPos,
		GetLastCursorPos(),
		PressedMouseButtons,
		Key,
		0
		);

    PressedMouseButtons.erase(MouseEvent.GetEffectingButton());

	return ProcessMouseButtonUpEvent( MouseEvent );
}

bool WindowsApplicationMessageHandler::OnMouseWheel( const float Delta )
{
    POINT p;
    GetCursorPos(&p);
	return OnMouseWheel(Delta, FVector2D(p.x,p.y));
}

bool WindowsApplicationMessageHandler::OnMouseWheel( const float Delta, const FVector2D CursorPos )
{
	FPointerEvent MouseWheelEvent(
		CursorPos,
		CursorPos,
		PressedMouseButtons,
		EKeys::Invalid,
		Delta
		);

	return ProcessMouseWheelOrGestureEvent( MouseWheelEvent, nullptr );
}

bool WindowsApplicationMessageHandler::OnMouseMove()
{
	bool Result = true;
	const FVector2D CurrentCursorPosition = GetCursorPos();
	const FVector2D LastCursorPosition = GetLastCursorPos();
	
	// Force the cursor user index to use the platform cursor since we've been notified that the platform 
	// cursor position has changed. This is done intentionally after getting the positions in order to avoid
	// false positives.

	// NOTE: When we swap out the real OS cursor for the faux slate cursor ie. UsePlatformCursorForCursorUser(false)
	// we reset this event count to 0.  This occurs typically when a gamepad is being used and you don't want to manipulate the real
	// OS cursor, instead move around a fake cursor so that you can still do development stuff outside the game window.  Anyway,
	// when this occurs, in the future the OS will send you a long delayed mouse movement.  I'm not exactly sure what's triggering it,
	// it's not a movement from the application, I think it's something more subtle, like swapping true cursor visibility for using
	// a None cursor, it could also be some combination.
	// 
	// In any event, we track the number of events, and if we get more than 3, then we start trying to swap back to the OS cursor.
	// the 3 should give us any buffer needed for either a last frame mouse movement, or a weird condition like noted above.
	//PlatformMouseMovementEvents++;

	//if (PlatformMouseMovementEvents > 3)
	//{
	//	UsePlatformCursorForCursorUser(true);
	//}
	
	if ( LastCursorPosition != CurrentCursorPosition )
	{
		FPointerEvent MouseEvent(
			CurrentCursorPosition,
			LastCursorPosition,
			PressedMouseButtons,
			EKeys::Invalid,
			0
			);

		Result = ProcessMouseMoveEvent( MouseEvent );
	}

	return Result;
}

bool WindowsApplicationMessageHandler::OnRawMouseMove( const int32 X, const int32 Y )
{
	if ( X != 0 || Y != 0 )
	{
		FPointerEvent MouseEvent(
			GetCursorPos(),
			GetLastCursorPos(),
			FVector2D( X, Y ), 
			PressedMouseButtons
		);

		ProcessMouseMoveEvent(MouseEvent);
	}
	
	return true;
}

void WindowsApplicationMessageHandler::OnResizingWindow( const std::shared_ptr< FGenericWindow >& PlatformWindow )
{
	// Flush the rendering command queue to ensure that there aren't pending viewport draw commands for the old viewport size.
	//Renderer->FlushCommands();
}

void WindowsApplicationMessageHandler::OnMovedWindow( const std::shared_ptr< FGenericWindow >& PlatformWindow, const int32 X, const int32 Y )
{

}

bool WindowsApplicationMessageHandler::OnApplicationActivationChanged(const bool InAppActivated)
{
	const bool UserSwitchedAway = bAppIsActive && !InAppActivated;
	const bool StateChanged = bAppIsActive != InAppActivated;

	bAppIsActive = InAppActivated;

	// If the user switched to a different application then we should dismiss our pop-ups.  In the case
	// where a user clicked on a different Slate window, OnWindowActivatedMessage() will be call MenuStack.OnWindowActivated()
	// to destroy any windows in our stack that are no longer appropriate to be displayed.
	if (UserSwitchedAway)
	{
		// Clear the pressed buttons when we deactivate the application, the button state can no longer be trusted.
		PressedMouseButtons.clear();
	}
	return true;
}

void WindowsApplicationMessageHandler::OnWindowClose(const std::shared_ptr<FGenericWindow>& PlatformWindow)
{
	
}

void WindowsApplicationMessageHandler::SetCursorPos( const FVector2D& MouseCoordinate )
{
    ::SetCursorPos(MouseCoordinate.x,MouseCoordinate.y);
}
