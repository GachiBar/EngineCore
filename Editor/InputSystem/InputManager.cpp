#include "InputManager.h"

#include <windowsx.h>

#include "../Application.h"
#include "UnrealCoreSystem/Windows/WindowsWindow.h"


InputManager& InputManager::getInstance()
{
	static InputManager instance;
	// Instantiated on first use.
	return instance;
}

Mouse& InputManager::GetMouseDevice()
{
	return mouse;
}

bool InputManager::IsInputMessage(UINT msg)
{
	if (IsMouseMessage(msg) || IsKeyboardMessage(msg))
	{
		return true;
	}
	return false;
}

bool InputManager::IsMouseMessage(UINT msg)
{
	switch (msg)
	{
	case WM_MOUSEHWHEEL:
	case WM_MOUSEWHEEL:
	case WM_MOUSEHOVER:
	case WM_MOUSELEAVE:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_RBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_XBUTTONDBLCLK:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
		return true;
	default:
		return false;
	}
}

bool InputManager::IsKeyboardMessage(UINT msg)
{
	switch (msg)
	{
	// Keyboard input notification messages...
	case WM_CHAR:
	case WM_SYSCHAR:
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
	case WM_SYSKEYUP:
	case WM_KEYUP:
	case WM_SYSCOMMAND:
		return true;
	default:
		return false;
	}
}

void InputManager::ProcessInput(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_KILLFOCUS)
		Flush();
	else if (msg == WM_INPUTLANGCHANGE || msg == WM_INPUTLANGCHANGEREQUEST)
		message_handler->OnInputLanguageChanged();
	else if (msg == WM_CHAR)
	{
		// Character code is stored in WPARAM
		const TCHAR Character = wparam;

		// lparam bit 30 will be ZERO for new presses, or ONE if this is a repeat
		const bool bIsRepeat = (lparam & 0x40000000) != 0;

		message_handler->OnKeyChar(Character, bIsRepeat);
	}
	else if (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN)
	{
		// Character code is stored in WPARAM
		const int Win32Key = wparam;

		// The actual key to use.  Some keys will be translated into other keys. 
		// I.E VK_CONTROL will be translated to either VK_LCONTROL or VK_RCONTROL as these
		// keys are never sent on their own
		int ActualKey = Win32Key;

		// lparam bit 30 will be ZERO for new presses, or ONE if this is a repeat
		bool bIsRepeat = (lparam & 0x40000000) != 0;

		switch (Win32Key)
		{
		case VK_MENU:
			// Differentiate between left and right alt
			if ((lparam & 0x1000000) == 0)
			{
				ActualKey = VK_LMENU;
				//bIsRepeat = ModifierKeyState[EModifierKey::LeftAlt];
				//ModifierKeyState[EModifierKey::LeftAlt] = true;
			}
			else
			{
				ActualKey = VK_RMENU;
				//bIsRepeat = ModifierKeyState[EModifierKey::RightAlt];
				//ModifierKeyState[EModifierKey::RightAlt] = true;
			}
			break;
		case VK_CONTROL:
			// Differentiate between left and right control
			if ((lparam & 0x1000000) == 0)
			{
				ActualKey = VK_LCONTROL;
				//bIsRepeat = ModifierKeyState[EModifierKey::LeftControl];
				//ModifierKeyState[EModifierKey::LeftControl] = true;
			}
			else
			{
				ActualKey = VK_RCONTROL;
				//bIsRepeat = ModifierKeyState[EModifierKey::RightControl];
				//ModifierKeyState[EModifierKey::RightControl] = true;
			}
			break;
		case VK_SHIFT:
			// Differentiate between left and right shift
			ActualKey = MapVirtualKey((lparam & 0x00ff0000) >> 16, MAPVK_VSC_TO_VK_EX);
			break;
		case VK_CAPITAL:
			//ModifierKeyState[EModifierKey::CapsLock] = (::GetKeyState(VK_CAPITAL) & 0x0001) != 0;
			break;
		default:
			// No translation needed
			break;
		}

		// Get the character code from the virtual key pressed.  If 0, no translation from virtual key to character exists
		const unsigned int CharCode = ::MapVirtualKey(Win32Key, MAPVK_VK_TO_CHAR);

		message_handler->OnKeyDown(ActualKey, CharCode, bIsRepeat);
	}
	else if (msg == WM_KEYUP || msg == WM_SYSKEYUP)
	{
		// Character code is stored in WPARAM
		int Win32Key = wparam;

		// The actual key to use.  Some keys will be translated into other keys. 
		// I.E VK_CONTROL will be translated to either VK_LCONTROL or VK_RCONTROL as these
		// keys are never sent on their own
		int ActualKey = Win32Key;

		switch (Win32Key)
		{
		case VK_MENU:
			// Differentiate between left and right alt
			if ((lparam & 0x1000000) == 0)
			{
				ActualKey = VK_LMENU;
				//ModifierKeyState[EModifierKey::LeftAlt] = false;
			}
			else
			{
				ActualKey = VK_RMENU;
				//ModifierKeyState[EModifierKey::RightAlt] = false;
			}
			break;
		case VK_CONTROL:
			// Differentiate between left and right control
			if ((lparam & 0x1000000) == 0)
			{
				ActualKey = VK_LCONTROL;
				//ModifierKeyState[EModifierKey::LeftControl] = false;
			}
			else
			{
				ActualKey = VK_RCONTROL;
				//ModifierKeyState[EModifierKey::RightControl] = false;
			}
			break;
		case VK_SHIFT:
			// Differentiate between left and right shift
			ActualKey = MapVirtualKey((lparam & 0x00ff0000) >> 16, MAPVK_VSC_TO_VK_EX);
			break;
		case VK_CAPITAL:
			//ModifierKeyState[EModifierKey::CapsLock] = (::GetKeyState(VK_CAPITAL) & 0x0001) != 0;
			break;
		default:
			// No translation needed
			break;
		}

		// Get the character code from the virtual key pressed.  If 0, no translation from virtual key to character exists
		unsigned int CharCode = ::MapVirtualKey(Win32Key, MAPVK_VK_TO_CHAR);

		// Key up events are never repeats
		const bool bIsRepeat = false;

		message_handler->OnKeyUp(ActualKey, CharCode, bIsRepeat);
	}
	//GetKeyboardDevice().OnKeyReleased(static_cast<unsigned char>(wparam));
	// ************ END KEYBOARD MESSAGES ************ //

	// ************ MOUSE MESSAGES ************ //
	else if (IsMouseMessage(msg))
	{
		if(msg == WM_MOUSEMOVE || msg==WM_NCMOUSEMOVE)
		{
			POINT CursorPoint;
			CursorPoint.x = GET_X_LPARAM(lparam);
			CursorPoint.y = GET_Y_LPARAM(lparam);

			message_handler->OnRawMouseMove(CursorPoint.x, CursorPoint.y);
		}
		else
		{
			POINT CursorPoint;
			CursorPoint.x = GET_X_LPARAM(lparam);
			CursorPoint.y = GET_Y_LPARAM(lparam);

			ClientToScreen(hwnd, &CursorPoint);

			const FVector2D CursorPos(CursorPoint.x, CursorPoint.y);

			EMouseButtons::Type MouseButton = EMouseButtons::Invalid;
			bool bDoubleClick = false;
			bool bMouseUp = false;
			switch (msg)
			{
			case WM_LBUTTONDBLCLK:
				bDoubleClick = true;
				MouseButton = EMouseButtons::Left;
				break;
			case WM_LBUTTONUP:
				bMouseUp = true;
				MouseButton = EMouseButtons::Left;
				break;
			case WM_LBUTTONDOWN:
				MouseButton = EMouseButtons::Left;
				break;
			case WM_MBUTTONDBLCLK:
				bDoubleClick = true;
				MouseButton = EMouseButtons::Middle;
				break;
			case WM_MBUTTONUP:
				bMouseUp = true;
				MouseButton = EMouseButtons::Middle;
				break;
			case WM_MBUTTONDOWN:
				MouseButton = EMouseButtons::Middle;
				break;
			case WM_RBUTTONDBLCLK:
				bDoubleClick = true;
				MouseButton = EMouseButtons::Right;
				break;
			case WM_RBUTTONUP:
				bMouseUp = true;
				MouseButton = EMouseButtons::Right;
				break;
			case WM_RBUTTONDOWN:
				MouseButton = EMouseButtons::Right;
				break;
			case WM_XBUTTONDBLCLK:
				bDoubleClick = true;
				MouseButton = (HIWORD(wparam) & XBUTTON1) ? EMouseButtons::Thumb01 : EMouseButtons::Thumb02;
				break;
			case WM_XBUTTONUP:
				bMouseUp = true;
				MouseButton = (HIWORD(wparam) & XBUTTON1) ? EMouseButtons::Thumb01 : EMouseButtons::Thumb02;
				break;
			case WM_XBUTTONDOWN:
				MouseButton = (HIWORD(wparam) & XBUTTON1) ? EMouseButtons::Thumb01 : EMouseButtons::Thumb02;
				break;
			}

			if (bMouseUp)
			{
				message_handler->OnMouseUp(MouseButton, CursorPos);
			}
			else if (bDoubleClick)
			{
				message_handler->OnMouseDoubleClick(app->GetMainWindow(), MouseButton, CursorPos);
			}
			else
			{
				message_handler->OnMouseDown(app->GetMainWindow(), MouseButton, CursorPos);
			}
		}
		
	}
	else if (msg == WM_MOUSEWHEEL)
	{
		const float SpinFactor = 1 / 120.0f;
		const SHORT WheelDelta = GET_WHEEL_DELTA_WPARAM(wparam);

		POINT CursorPoint;
		CursorPoint.x = GET_X_LPARAM(lparam);
		CursorPoint.y = GET_Y_LPARAM(lparam);

		const FVector2D CursorPos(CursorPoint.x, CursorPoint.y);

		message_handler->OnMouseWheel(static_cast<float>(WheelDelta) * SpinFactor, CursorPos);
	}
	else if (msg == WM_CLOSE)
	{
		if (app->GetMainWindow())
		{
			// Called when the OS close button is pressed
			message_handler->OnWindowClose(app->GetMainWindow());
		}
	}
	else if(msg == WM_SIZING)
	{
		if (app->GetMainWindow())
		{
			// @todo Fullscreen - Perform deferred resize
			// Note WM_SIZE provides the client dimension which is not equal to the window dimension if there is a windows border 
			const int32 NewWidth = (int)(short)(LOWORD(lparam));
			const int32 NewHeight = (int)(short)(HIWORD(lparam));

			const FGenericWindowDefinition& Definition = app->GetMainWindow()->GetDefinition();
			if (Definition.IsRegularWindow && !Definition.HasOSWindowBorder)
			{
				static_cast<FWindowsWindow*>(app->GetMainWindow().get())->AdjustWindowRegion(NewWidth, NewHeight);
			}

			const bool bWasMinimized = (wparam == SIZE_MINIMIZED);

			const bool bIsFullscreen = (app->GetMainWindow()->GetWindowMode() == EWindowMode::Type::Fullscreen);

			// When in fullscreen Windows rendering size should be determined by the application. Do not adjust based on WM_SIZE messages.
			if (!bIsFullscreen)
			{
				message_handler->OnSizeChanged(app->GetMainWindow(), NewWidth, NewHeight, bWasMinimized);
			}
		}
	}
	else if(msg == WM_SIZE)
	{
	const bool bIsFullscreen = (app->GetMainWindow()->GetWindowMode() == EWindowMode::Type::Fullscreen);

	// When in fullscreen Windows rendering size should be determined by the application. Do not adjust based on WM_SIZE messages.
		if (!bIsFullscreen)
		{
			message_handler->OnResizingWindow(app->GetMainWindow());
		}
		
	}
	// ************ END MOUSE MESSAGES ************ //

	/*
	 	else if (msg == WM_MOUSEMOVE)
	{
		uint32 Size = 0;
		::GetRawInputData((HRAWINPUT)lparam, RID_INPUT, NULL, &Size, sizeof(RAWINPUTHEADER));
		std::vector<uint8> RawData(Size);

		if (::GetRawInputData((HRAWINPUT)lparam, RID_INPUT, RawData.data(), &Size, sizeof(RAWINPUTHEADER)) == Size)
		{
			const auto Raw = reinterpret_cast<const RAWINPUT* const>(RawData.data());

			if (Raw->header.dwType == RIM_TYPEMOUSE)
			{
				const bool IsAbsoluteInput = (Raw->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE) == MOUSE_MOVE_ABSOLUTE;
				if (IsAbsoluteInput)
				{
					// Since the raw input is coming in as absolute it is likely the user is using a tablet
					// or perhaps is interacting through a virtual desktop
					// Absolute coordinates given through raw input are simulated using MouseMove to get relative coordinates
					message_handler->OnMouseMove();
				}
			}
			// Since raw input is coming in as relative it is likely a traditional mouse device
			const int xPosRelative = Raw->data.mouse.lLastX;
			const int yPosRelative = Raw->data.mouse.lLastY;
			message_handler->OnRawMouseMove(xPosRelative, yPosRelative);
		}
	}
	else if (msg == WM_MOUSEMOVE)
	{
		POINTS pt = MAKEPOINTS(lparam);
		//TODO remove magic numbers
		if (pt.x > 0 && pt.x < 800 && pt.y > 0 && pt.y < 600)
		{
			GetMouseDevice().OnMouseMove(pt.x, pt.y);
			if (!GetMouseDevice().IsInWindow())
			{
				SetCapture(hwnd);
				GetMouseDevice().OnMouseEnter();
			}
		}
		else
		{
			if (wparam & (MK_LBUTTON | MK_RBUTTON))
			{
				pt.x = std::max(short(0), pt.x);
				//TODO remove magic numbers
				pt.x = std::min(short(800 - 1), pt.x);
				pt.y = std::max(short(0), pt.y);
				//TODO remove magic numbers
				pt.y = std::min(short(600 - 1), pt.y);
				GetMouseDevice().OnMouseMove(pt.x, pt.y);
			}
			else
			{
				ReleaseCapture();
				GetMouseDevice().OnMouseLeave();
				GetMouseDevice().OnLeftReleased();
				GetMouseDevice().OnRightReleased();
			}
		}
	}
	 */
}

std::pair<float, float> InputManager::GetMousePosition() const
{
	return mouse.GetPos();
}

bool InputManager::ReadEvent(FInputEvent& IE) const
{
	return player_input->MessageHandler->ReadInputEvent(IE);
}

InputManager::InputManager(): app(nullptr), player_input(nullptr), message_handler(nullptr)
{
}

void InputManager::Flush()
{
	//TODO
	//KeyStates flush

	mouse.Flush();
}
