#include "WindowsApplicationMessageHandler.h"
#include "Windows.h"
#include "../InputEvent/Events.h"
#include "../KeyStruct.h"
#include "../Keys.h"
#include "../InputKeyManager.h"
#include "../../InputManager.h"

static const FKey& TranslateMouseButtonToKey(const EMouseButtons::Type Button)
{
	switch (Button)
	{
	case EMouseButtons::Left:
		return EKeys::LeftMouseButton;

	case EMouseButtons::Middle:
		return EKeys::MiddleMouseButton;

	case EMouseButtons::Right:
		return EKeys::RightMouseButton;

	case EMouseButtons::Thumb01:
		return EKeys::ThumbMouseButton;

	case EMouseButtons::Thumb02:
		return EKeys::ThumbMouseButton2;

	default:
		return EKeys::Invalid;
	}
}

bool WindowsApplicationMessageHandler::OnKeyChar(const wchar_t Character, const bool IsRepeat)
{
	FCharacterEvent CharacterEvent(Character, IsRepeat);
	InputStack.push(std::make_shared<FCharacterEvent>(CharacterEvent));
	return true;
}

bool WindowsApplicationMessageHandler::OnKeyDown(const int32 KeyCode, const uint32 CharacterCode, const bool IsRepeat, const bool IsModified)
{
	const FKey Key = InputKeyManager::Get().GetKeyFromCodes(KeyCode, CharacterCode, IsModified && CharacterCode != 0);
	const FKeyEvent KeyEvent(Key, IsRepeat, IsRepeat ? IE_Repeat : IE_Pressed, CharacterCode, KeyCode);

	InputStack.push(std::make_shared<FKeyEvent>(KeyEvent));

	return true;
}

bool WindowsApplicationMessageHandler::OnKeyUp(const int32 KeyCode, const uint32 CharacterCode, const bool IsRepeat, const bool IsModified)
{
	const FKey Key = InputKeyManager::Get().GetKeyFromCodes(KeyCode, CharacterCode, IsModified && CharacterCode!=0);
	const FKeyEvent KeyEvent(Key, IsRepeat, IE_Released, CharacterCode, KeyCode);

	InputStack.push(std::make_shared<FKeyEvent>(KeyEvent));

	return true;
}

void WindowsApplicationMessageHandler::OnInputLanguageChanged()
{
	InputKeyManager::Get().InitKeyMappings();
}

bool WindowsApplicationMessageHandler::OnMouseDown(const std::shared_ptr<FGenericWindow>& PlatformWindow,
                                                   const EMouseButtons::Type Button)
{
	//TODO move it to some class
	POINT p;
	GetCursorPos(&p);

	return OnMouseDown(PlatformWindow, Button, FVector2D(p.x, p.y));
}

bool WindowsApplicationMessageHandler::OnMouseDown(const std::shared_ptr<FGenericWindow>& PlatformWindow,
                                                   const EMouseButtons::Type Button, const FVector2D CursorPos)
{
	const FKey& Key = TranslateMouseButtonToKey(Button);

	InputManager::getInstance().GetMouseDevice().AddPressedMouseButton(Key);

	const FPointerEvent MouseEvent(
		IE_Pressed,
		CursorPos,
		InputManager::getInstance().GetMouseDevice().GetLastPosFloatPoint(),
		InputManager::getInstance().GetMouseDevice().GetPressedButtons(),
		Key,
		0.f
	);

	InputStack.push(std::make_shared<FPointerEvent>(MouseEvent));
	return true;
}

bool WindowsApplicationMessageHandler::OnMouseDoubleClick(const std::shared_ptr<FGenericWindow>& PlatformWindow,
                                                          const EMouseButtons::Type Button)
{
	POINT p;
	GetCursorPos(&p);
	return OnMouseDoubleClick(PlatformWindow, Button, FVector2D{static_cast<float>(p.x), static_cast<float>(p.y)});
}

bool WindowsApplicationMessageHandler::OnMouseDoubleClick(const std::shared_ptr<FGenericWindow>& PlatformWindow,
                                                          const EMouseButtons::Type Button, const FVector2D CursorPos)
{
	const FKey& Key = TranslateMouseButtonToKey(Button);

	InputManager::getInstance().GetMouseDevice().AddPressedMouseButton(Key);

	const FPointerEvent MouseEvent(
		IE_DoubleClick,
		CursorPos,
		InputManager::getInstance().GetMouseDevice().GetLastPosFloatPoint(),
		InputManager::getInstance().GetMouseDevice().GetPressedButtons(),
		Key,
		0
	);

	InputStack.push(std::make_shared<FPointerEvent>(MouseEvent));

	return true;
}

bool WindowsApplicationMessageHandler::OnMouseUp(const EMouseButtons::Type Button)
{
	POINT p;
	GetCursorPos(&p);
	return OnMouseUp(Button, FVector2D{static_cast<float>(p.x), static_cast<float>(p.y)});
}

bool WindowsApplicationMessageHandler::OnMouseUp(const EMouseButtons::Type Button, const FVector2D CursorPos)
{
	const FKey& Key = TranslateMouseButtonToKey(Button);

	InputManager::getInstance().GetMouseDevice().RemoveMouseButton(Key);

	FPointerEvent MouseEvent(
		IE_Released,
		CursorPos,
		InputManager::getInstance().GetMouseDevice().GetLastPosFloatPoint(),
		InputManager::getInstance().GetMouseDevice().GetPressedButtons(),
		Key,
		0
	);

	InputStack.push(std::make_shared<FPointerEvent>(MouseEvent));
	return true;
}

bool WindowsApplicationMessageHandler::OnMouseWheel(const float Delta)
{
	POINT p;
	GetCursorPos(&p);
	return OnMouseWheel(Delta, FVector2D(p.x, p.y));
}

bool WindowsApplicationMessageHandler::OnMouseWheel(const float Delta, const FVector2D CursorPos)
{
	FPointerEvent MouseWheelEvent(
		IE_Axis,
		CursorPos,
		CursorPos,
		InputManager::getInstance().GetMouseDevice().GetPressedButtons(),
		EKeys::MouseWheelAxis,
		Delta
	);
	InputStack.push(std::make_shared<FPointerEvent>(MouseWheelEvent));

	//TODO Mouse wheel and up events
	FPointerEvent MouseWheelPressedEvent(MouseWheelEvent,
	                                     IE_Pressed,
	                                     Delta > 0 ? EKeys::MouseScrollUp : EKeys::MouseScrollDown
	);
	InputStack.push(std::make_shared<FPointerEvent>(MouseWheelPressedEvent));

	FPointerEvent MouseWheelReleasedEvent(MouseWheelEvent,
	                                      IE_Released,
	                                      Delta > 0 ? EKeys::MouseScrollUp : EKeys::MouseScrollDown
	);
	InputStack.push(std::make_shared<FPointerEvent>(MouseWheelReleasedEvent));

	return true;
}

bool WindowsApplicationMessageHandler::OnMouseMove()
{
	/*
	POINT p;
	GetCursorPos(&p);
	const FVector2D CurrentCursorPosition = {static_cast<float>(p.x), static_cast<float>(p.y)};

	const FVector2D LastCursorPosition = InputManager::getInstance().GetMouseDevice().GetCurPosFloatPoint();

	if (LastCursorPosition != CurrentCursorPosition)
	{
		if (LastCursorPosition.y != CurrentCursorPosition.y)
		{
			FPointerEvent MouseEventY(
				IE_Axis,
				CurrentCursorPosition,
				LastCursorPosition,
				InputManager::getInstance().GetMouseDevice().GetPressedButtons(),
				EKeys::MouseY,
				0
			);

			InputStack.push(std::make_shared<FPointerEvent>(MouseEventY));
		}
		if (LastCursorPosition.y != CurrentCursorPosition.y)
		{
			FPointerEvent MouseEventX(
				IE_Axis,
				CurrentCursorPosition,
				LastCursorPosition,
				InputManager::getInstance().GetMouseDevice().GetPressedButtons(),
				EKeys::MouseX,
				0
			);

			InputStack.push(std::make_shared<FPointerEvent>(MouseEventX));
		}

		InputManager::getInstance().GetMouseDevice().SetPos(p.x, p.y);
	}
	*/
	return true;
}

bool WindowsApplicationMessageHandler::OnRawMouseMove(const int32 X, const int32 Y)
{
	const FVector2D LastCursorPosition = InputManager::getInstance().GetMouseDevice().GetCurPosFloatPoint();
	
	if(X !=0)
	{
		FPointerEvent MouseEventX(IE_Axis,LastCursorPosition+FVector2D(X,Y),LastCursorPosition, InputManager::getInstance().GetMouseDevice().GetPressedButtons(),EKeys::MouseX,0);
		InputStack.push(std::make_shared<FPointerEvent>(MouseEventX));
	}
	if(Y!=0)
	{
		FPointerEvent MouseEventY(IE_Axis, LastCursorPosition + FVector2D(X, Y), LastCursorPosition, InputManager::getInstance().GetMouseDevice().GetPressedButtons(), EKeys::MouseY, 0);
		InputStack.push(std::make_shared<FPointerEvent>(MouseEventY));
	}
	InputManager::getInstance().GetMouseDevice().SetPos(LastCursorPosition.x+X, LastCursorPosition.y + Y);

	return true;
}

void WindowsApplicationMessageHandler::OnSizeChanged(const std::shared_ptr<FGenericWindow>& PlatformWindow,
                                                     const int32 Width, const int32 Height)
{
		PlatformWindow->OnSizeChanged(Width, Height);
}

void WindowsApplicationMessageHandler::OnResizingWindow(const std::shared_ptr<FGenericWindow>& PlatformWindow, bool bWasMinimized)
{
	// Flush the rendering command queue to ensure that there aren't pending viewport draw commands for the old viewport size.
	//Renderer->FlushCommands();
}

void WindowsApplicationMessageHandler::OnMovedWindow(const std::shared_ptr<FGenericWindow>& PlatformWindow,
                                                     const int32 X, const int32 Y)
{
}

bool WindowsApplicationMessageHandler::OnApplicationActivationChanged(const bool InAppActivated)
{
	const bool UserSwitchedAway = bAppIsActive && !InAppActivated;


	bAppIsActive = InAppActivated;

	// If the user switched to a different application then we should dismiss our pop-ups.  In the case
	// where a user clicked on a different Slate window, OnWindowActivatedMessage() will be call MenuStack.OnWindowActivated()
	// to destroy any windows in our stack that are no longer appropriate to be displayed.
	if (UserSwitchedAway)
	{
		// Clear the pressed buttons when we deactivate the application, the button state can no longer be trusted.
		InputManager::getInstance().GetMouseDevice().Flush();
	}
	return true;
}

void WindowsApplicationMessageHandler::OnWindowClose(const std::shared_ptr<FGenericWindow>& PlatformWindow)
{
	//TODO request exit
	//invalidate window ptr
}

void WindowsApplicationMessageHandler::SetCursorPos(const FVector2D& MouseCoordinate)
{
	::SetCursorPos(MouseCoordinate.x, MouseCoordinate.y);
}
