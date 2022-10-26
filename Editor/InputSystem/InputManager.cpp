#include "InputManager.h"

#include <windowsx.h>

#include "InputKeys.h"


InputManager& InputManager::getInstance()
{
	static InputManager instance;
	// Instantiated on first use.
	return instance;
}

Keyboard& InputManager::GetKeyboardDevice()
{
	return kbd;
}

Mouse& InputManager::GetMouseDevice()
{
	std::shared_ptr<int> t(new int);
	t.reset(new int);
	std::map<std::string, int> t1;

	std::erase_if(t1, [](auto& a) {return a.first; });
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
		GetKeyboardDevice().ClearState();
	// ************ KEYBOARD MESSAGES ************ //
	else if (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN)
	{
		// Character code is stored in WPARAM
				const int Win32Key = wparam;

				// The actual key to use.  Some keys will be translated into other keys. 
				// I.E VK_CONTROL will be translated to either VK_LCONTROL or VK_RCONTROL as these
				// keys are never sent on their own
				int ActualKey = Win32Key;

				// LPARAM bit 30 will be ZERO for new presses, or ONE if this is a repeat
				bool bIsRepeat = ( lparam & 0x40000000 ) != 0;

				switch( Win32Key )
				{
				case VK_MENU:
					// Differentiate between left and right alt
					if( (lparam & 0x1000000) == 0 )
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
					if( (lparam & 0x1000000) == 0 )
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
					ActualKey = MapVirtualKey( (lparam & 0x00ff0000) >> 16, MAPVK_VSC_TO_VK_EX);
					if (ActualKey == VK_LSHIFT)
					{
						//bIsRepeat = ModifierKeyState[EModifierKey::LeftShift];
						//ModifierKeyState[EModifierKey::LeftShift] = true;
					}
					else
					{
						//bIsRepeat = ModifierKeyState[EModifierKey::RightShift];
						//ModifierKeyState[EModifierKey::RightShift] = true;
					}
					break;
				case VK_CAPITAL:
					//ModifierKeyState[EModifierKey::CapsLock] = (::GetKeyState(VK_CAPITAL) & 0x0001) != 0;
					break;
				default:
					// No translation needed
					break;
				}

				// Get the character code from the virtual key pressed.  If 0, no translation from virtual key to character exists
				unsigned int CharCode = ::MapVirtualKey( Win32Key, MAPVK_VK_TO_CHAR );

				const bool Result = MessageHandler->OnKeyDown(ActualKey, CharCode, bIsRepeat);

				// Always return 0 to handle the message or else windows will beep
				if( Result || msg != WM_SYSKEYDOWN )
				{
					// Handled
					return;
				}
				/*
		if (!(lparam & 0x40000000) || GetKeyboardDevice().IsAutorepeatEnabled()) // no thank you on the autorepeat
		{
			GetKeyboardDevice().OnKeyPressed(static_cast<unsigned char>(wparam));
		}
		*/
			}
	else if (msg == WM_KEYUP || msg == WM_SYSKEYUP)
	{
		// Character code is stored in WPARAM
				int Win32Key = wparam;

				// The actual key to use.  Some keys will be translated into other keys. 
				// I.E VK_CONTROL will be translated to either VK_LCONTROL or VK_RCONTROL as these
				// keys are never sent on their own
				int ActualKey = Win32Key;

				bool bModifierKeyReleased = false;
				switch( Win32Key )
				{
				case VK_MENU:
					// Differentiate between left and right alt
					if( (lparam & 0x1000000) == 0 )
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
					if( (lparam & 0x1000000) == 0 )
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
					ActualKey = MapVirtualKey( (lparam & 0x00ff0000) >> 16, MAPVK_VSC_TO_VK_EX);
					if (ActualKey == VK_LSHIFT)
					{
						//ModifierKeyState[EModifierKey::LeftShift] = false;
					}
					else
					{
						//ModifierKeyState[EModifierKey::RightShift] = false;
					}
					break;
				case VK_CAPITAL:
					//ModifierKeyState[EModifierKey::CapsLock] = (::GetKeyState(VK_CAPITAL) & 0x0001) != 0;
					break;
				default:
					// No translation needed
					break;
				}

				// Get the character code from the virtual key pressed.  If 0, no translation from virtual key to character exists
				unsigned int CharCode = ::MapVirtualKey( Win32Key, MAPVK_VK_TO_CHAR );

				// Key up events are never repeats
				const bool bIsRepeat = false;

				const bool Result = MessageHandler->OnKeyUp( ActualKey, CharCode, bIsRepeat );

				// Note that we allow system keys to pass through to DefWndProc here, so that core features
				// like Alt+F4 to close a window work.
				if( Result || msg != WM_SYSKEYUP )
				{
					// Handled
					return;
				}
	}
		//GetKeyboardDevice().OnKeyReleased(static_cast<unsigned char>(wparam));
	// ************ END KEYBOARD MESSAGES ************ //

	// ************ MOUSE MESSAGES ************ //
	else if(msg ==  WM_MOUSEMOVE)
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
	else if (msg == WM_LBUTTONDOWN)
	{
		const POINTS pt = MAKEPOINTS(lparam);
		GetMouseDevice().OnLeftPressed();
		SetForegroundWindow(hwnd);
	}
	else if (msg == WM_RBUTTONDOWN)
	{
		const POINTS pt = MAKEPOINTS(lparam);
		GetMouseDevice().OnRightPressed();
	}
	else if (msg == WM_LBUTTONUP)
	{
		const POINTS pt = MAKEPOINTS(lparam);
		GetMouseDevice().OnLeftReleased();
	}
	else if (msg == WM_RBUTTONUP)
	{
		const POINTS pt = MAKEPOINTS(lparam);
		GetMouseDevice().OnRightReleased();
	}
	else if (msg == WM_MOUSEWHEEL)
	{
		short const delta = GET_WHEEL_DELTA_WPARAM(wparam);
		const POINTS pt = MAKEPOINTS(lparam);

		const float SpinFactor = 1 / 120.0f;
		const SHORT WheelDelta = GET_WHEEL_DELTA_WPARAM(wparam);

		POINT CursorPoint;
		CursorPoint.x = GET_X_LPARAM(lparam);
		CursorPoint.y = GET_Y_LPARAM(lparam);

		const std::pair CursorPos(CursorPoint.x, CursorPoint.y);

		//const BOOL Result = MessageHandler->OnMouseWheel(static_cast<float>(WheelDelta) * SpinFactor, CursorPos);
		//return Result ? 0 : 1;
		float delta2 = static_cast<float>(WheelDelta) * SpinFactor;

		if (delta2 > 0)
		{
			GetMouseDevice().OnWheelUp(copysignf(1.f, delta2));
		}
		else if (delta2 < 0)
		{
			GetMouseDevice().OnWheelDown(copysignf(1.f, delta2));
		}
	}
	// ************ END MOUSE MESSAGES ************ //
}

void InputManager::SendEventToBuffer(std::shared_ptr<InputEvent> const & input_event)
{
	input_buffer.push(input_event);
	TrimBuffer(input_buffer);
}

void InputManager::SetInputBufferSize(int new_buffer_size)
{
	bufferSize = new_buffer_size;
}

std::shared_ptr<InputEvent> InputManager::ReadEvent()
{
	if (!input_buffer.empty())
	{
		const auto input = input_buffer.front();
		input_buffer.pop();
		return input;
	}
	return nullptr;
}

std::pair<float, float> InputManager::GetMousePosition() const
{
	return mouse.GetPos();
}

float InputManager::GetWheelDelta() const
{
	return mouse.GetDelta();
}

bool InputManager::GetKey(KeyboardKey key) const
{
	return kbd.KeyIsPressed(static_cast<unsigned char>(key));
}

bool InputManager::GetMouseKey(MouseKey key) const
{
	return mouse.IsKeyPressed(key);
}

bool InputManager::GetKeyDown(KeyboardKey key) const
{
	return kbd.IsKeyDown(key);
}

bool InputManager::GetKeyUp(KeyboardKey key) const
{
	return kbd.IsKeyUp(key);
}

bool InputManager::GetMouseKeyDown(MouseKey key) const
{
	return mouse.IsPressedDown(key);
}

bool InputManager::GetMouseKeyUp(MouseKey key) const
{
	return mouse.IsPressedUp(key);
}

void InputManager::Flush()
{
	kbd.Flush();
	mouse.Flush();
}

InputManager::InputManager():bufferSize(16)
{}

template <typename T>
void InputManager::TrimBuffer(std::queue<T>& buffer)
{
	while (bufferSize != 0 && buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}