#include "InputManager.h"

#include <iostream>
#include <windowsx.h>

#include "../Core/Windows/WindowsWindow.h"
#include "../Core/Interfaces/IApplication.h"
#include "InputCoreSystem/InputSettings.h"

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
	if (IsMouseMessage(msg) || IsKeyboardMessage(msg) || msg == WM_INPUT || msg==WM_SIZE || msg==WM_SIZING)
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

bool InputManager::IsMouseButton(UINT msg)
{
	switch (msg)
	{
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
	if(msg==WM_INPUT)
	{
		UINT dwSize = 0;
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));
		LPBYTE lpb = new BYTE[dwSize];
		if (lpb == nullptr) {
			return;
		}
		if (GetRawInputData((HRAWINPUT)lparam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
			OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));

		RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(lpb);

		if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			GetMessageHandler()->OnRawMouseMove(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
			delete[] lpb;
			return;
		}

		delete[] lpb;
	}

	if (msg == WM_KILLFOCUS)
		Flush();
	else if (msg == WM_INPUTLANGCHANGE || msg == WM_INPUTLANGCHANGEREQUEST)
		GetMessageHandler()->OnInputLanguageChanged();
	else if (msg == WM_CHAR)
	{
		// Character code is stored in WPARAM
		const TCHAR Character = wparam;

		// lparam bit 30 will be ZERO for new presses, or ONE if this is a repeat
		const bool bIsRepeat = (lparam & 0x40000000) != 0;

		GetMessageHandler()->OnKeyChar(Character, bIsRepeat);
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
			}
			else
			{
				ActualKey = VK_RMENU;
			}
			break;
		case VK_CONTROL:
			// Differentiate between left and right control
			if ((lparam & 0x1000000) == 0)
			{
				ActualKey = VK_LCONTROL;
			}
			else
			{
				ActualKey = VK_RCONTROL;
			}
			break;
		case VK_SHIFT:
			// Differentiate between left and right shift
			ActualKey = MapVirtualKey((lparam & 0x00ff0000) >> 16, MAPVK_VSC_TO_VK_EX);
			break;
		default:
			// No translation needed
			break;
		}

		// Get the character code from the virtual key pressed.  If 0, no translation from virtual key to character exists
		const unsigned int CharCode = TranslateVirtualKeyToCharCode(Win32Key);

		GetMessageHandler()->OnKeyDown(ActualKey, CharCode, bIsRepeat, IsShiftPressed() && Win32Key != VK_SHIFT);
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
			}
			else
			{
				ActualKey = VK_RMENU;
			}
			break;
		case VK_CONTROL:
			// Differentiate between left and right control
			if ((lparam & 0x1000000) == 0)
			{
				ActualKey = VK_LCONTROL;
			}
			else
			{
				ActualKey = VK_RCONTROL;
			}
			break;
		case VK_SHIFT:
			// Differentiate between left and right shift
			ActualKey = MapVirtualKey((lparam & 0x00ff0000) >> 16, MAPVK_VSC_TO_VK_EX);
			break;
		default:
			// No translation needed
			break;
		}

		// Get the character code from the virtual key pressed.  If 0, no translation from virtual key to character exists
		const unsigned int CharCode = TranslateVirtualKeyToCharCode(Win32Key);

		// Key up events are never repeats
		const bool bIsRepeat = false;

		GetMessageHandler()->OnKeyUp(ActualKey, CharCode, bIsRepeat,IsShiftPressed() && Win32Key!=VK_SHIFT);
	}
	// ************ END KEYBOARD MESSAGES ************ //

	// ************ MOUSE MESSAGES ************ //
	else if (IsMouseMessage(msg))
	{
		if (msg == WM_MOUSEMOVE || msg == WM_NCMOUSEMOVE)
		{
			POINT CursorPoint;
			CursorPoint.x = GET_X_LPARAM(lparam);
			CursorPoint.y = GET_Y_LPARAM(lparam);


			//GetMessageHandler()->OnMouseMove();
		}
		else if (msg == WM_MOUSEWHEEL || msg == WM_MOUSEHWHEEL)
		{
			const float SpinFactor = 1 / 120.0f;
			const SHORT WheelDelta = GET_WHEEL_DELTA_WPARAM(wparam);

			POINT CursorPoint;
			CursorPoint.x = GET_X_LPARAM(lparam);
			CursorPoint.y = GET_Y_LPARAM(lparam);

			const FVector2D CursorPos(CursorPoint.x, CursorPoint.y);

			GetMessageHandler()->OnMouseWheel(static_cast<float>(WheelDelta) * SpinFactor, CursorPos);
		}
		else if (IsMouseButton(msg))
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
				GetMessageHandler()->OnMouseUp(MouseButton, CursorPos);
			}
			else if (bDoubleClick)
			{
				GetMessageHandler()->OnMouseDoubleClick(app->GetMainWindow(), MouseButton, CursorPos);
			}
			else
			{
				GetMessageHandler()->OnMouseDown(app->GetMainWindow(), MouseButton, CursorPos);
			}
		}
	}
	else if (msg == WM_CLOSE)
	{
		if (app->GetMainWindow())
		{
			// Called when the OS close button is pressed
			GetMessageHandler()->OnWindowClose(app->GetMainWindow());
		}
	}
	else if (msg == WM_SIZING)
	{
		if (app->GetMainWindow())
		{
			PRECT rectp = (PRECT)lparam;

			int width = rectp->right-rectp->left;
			int height = rectp->bottom - rectp->top;


			const FGenericWindowDefinition& Definition = app->GetMainWindow()->GetDefinition();
			if (Definition.IsRegularWindow && !Definition.HasOSWindowBorder)
			{
				static_cast<FWindowsWindow*>(app->GetMainWindow().get())->AdjustWindowRegion(width, height);
			}

			const bool bWasMinimized = (wparam == SIZE_MINIMIZED);

			const bool bIsFullscreen = (app->GetMainWindow()->GetWindowMode() == EWindowMode::Type::Fullscreen);

			// When in fullscreen Windows rendering size should be determined by the application. Do not adjust based on WM_SIZE messages.
			if (!bIsFullscreen)
			{
				GetMessageHandler()->OnSizeChanged(app->GetMainWindow(), width, height);
			}
		}
	}
	else if (msg == WM_SIZE)
	{
		if(app && app->GetMainWindow())
		{
			const bool bIsFullscreen = (app->GetMainWindow()->GetWindowMode() == EWindowMode::Type::Fullscreen);

			// When in fullscreen Windows rendering size should be determined by the application. Do not adjust based on WM_SIZE messages.
			if (!bIsFullscreen)
			{
				const bool bWasMinimized = (wparam == SIZE_MINIMIZED);

				GetMessageHandler()->OnResizingWindow(app->GetMainWindow(),bWasMinimized);
			}
		}
	}
	// ************ END MOUSE MESSAGES ************ //
}

std::pair<float, float> InputManager::GetMousePosition() const
{
	return mouse.GetPos();
}

bool InputManager::ReadEvent(std::shared_ptr<FInputEvent>& IE) const
{
	return player_input->MessageHandler->ReadInputEvent(IE);
}

PlayerInput* InputManager::GetPlayerInput() const
{
	return player_input.get();
}

void InputManager::SetPlayerInput(PlayerInput* InPlayerEnums)
{
	player_input.reset(InPlayerEnums);
}

FGenericApplicationMessageHandler* InputManager::GetMessageHandler()
{
	return player_input->MessageHandler.get();
}

void InputManager::RegisterInputDevice(IApplication* InApp)
{
	app = InApp;

	RAWINPUTDEVICE Rid[2];

	Rid[0].usUsagePage = 0x01;
	Rid[0].usUsage = 0x02;
	Rid[0].dwFlags = 0; // adds HID mouse and also ignores legacy mouse messages
	Rid[0].hwndTarget = static_cast<FWindowsWindow*>(app->GetMainWindow().get())->GetHWnd();

	Rid[1].usUsagePage = 0x01;
	Rid[1].usUsage = 0x06;
	Rid[1].dwFlags = 0; // adds HID keyboard and also ignores legacy keyboard messages
	Rid[1].hwndTarget = static_cast<FWindowsWindow*>(app->GetMainWindow().get())->GetHWnd();

	if (RegisterRawInputDevices(Rid, 2, sizeof(Rid[0])) == FALSE)
	{
		const auto errorCode = GetLastError();
		std::cout << "ERROR: " << errorCode << std::endl;
	}
}

IApplication* InputManager::GetOwningApp() const
{
	return app;
}

InputManager::InputManager() : app(nullptr),
                               input_settings(new InputSettings()),
                               player_input(new PlayerInput())
{
	player_input->SetInputSettings(input_settings.get());
}

bool InputManager::IsShiftPressed() const
{
	return player_input->IsPressed(EKeys::LeftShift);
}

unsigned InputManager::TranslateVirtualKeyToCharCode(int wParam) const
{
	unsigned int result = ::MapVirtualKey(wParam, MAPVK_VK_TO_CHAR);

	if (IsShiftPressed()) // Check if shift key is down (fairly accurate)
	{
		if (wParam >= '0' && wParam <= '9') // Keys 0-9
		{
			switch (wParam)
				// 0x30-0x39 is 0-9 respectively
			{
			case 0x30:
				result = ')';
				break;
			case 0x31:
				result = '!';
				break;
			case 0x32:
				result = '@';
				break;
			case 0x33:
				result = '#';
				break;
			case 0x34:
				result = '$';
				break;
			case 0x35:
				result = '%';
				break;
			case 0x36:
				result = '^';
				break;
			case 0x37:
				result = '&';
				break;
			case 0x38:
				result = '*';
				break;
			case 0x39:
				result = '(';
				break;
			default:;
			}
		}
	}
	switch (wParam)
		// Check for other keys
	{
	case VK_OEM_2:
		if (IsShiftPressed())
			result = '?';
		else
			result = '/';
		break;
	case VK_OEM_3:
		if (IsShiftPressed())
			result = '~';
		else
			result = '`';
		break;
	case VK_OEM_4:
		if (IsShiftPressed())
			result = '{';
		else
			result = '[';
		break;
	case VK_OEM_5:
		if (IsShiftPressed())
			result = '|';
		else
			result = '\\';
		break;
	case VK_OEM_6:
		if (IsShiftPressed())
			result = '}';
		else
			result = ']';
		break;
	case VK_OEM_7:
		if (IsShiftPressed())
			result = '\"';
		else
			result = '\'';
		break;
	case 0xBC: //comma
		if (IsShiftPressed())
			result = '<';
		else
			result = ',';
		break;
	case 0xBE: //Period
		if (IsShiftPressed())
			result = '>';
		else
			result = '.';
		break;
	case 0xBA: //Semi Colon same as VK_OEM_1
		if (IsShiftPressed())
			result = ':';
		else
			result = ';';
		break;
	case 0xBD: //Minus
		if (IsShiftPressed())
			result = '_';
		else
			result = '-';
		break;
	case 0xBB: //Equal
		if (IsShiftPressed())
			result = '+';
		else
			result = '=';
		break;
	default: ;
	}
	return result;
}


void InputManager::Flush()
{
	//KeyStates flush
	for (auto& KeyState : player_input->GetKeyStates())
	{
		KeyState.second.FlushEvents();
	}

	mouse.Flush();
}
