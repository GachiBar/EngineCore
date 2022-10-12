#include "InputManager.h"


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
	return mouse;
}

bool InputManager::IsInputMessage(UINT msg) const
{
	return msg == WM_KILLFOCUS || msg == WM_KEYDOWN || msg == WM_KEYUP || msg == WM_CHAR || msg == WM_MOUSEMOVE || msg == WM_LBUTTONDOWN
		|| msg == WM_RBUTTONDOWN || msg == WM_LBUTTONUP || msg == WM_RBUTTONUP || msg == WM_MOUSEWHEEL;
}

void InputManager::ProcessInput(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_KILLFOCUS)
		GetKeyboardDevice().ClearState();
	// ************ KEYBOARD MESSAGES ************ //
	else if (msg == WM_KEYDOWN)
	{
		if (!(lparam & 0x40000000) || GetKeyboardDevice().IsAutorepeatEnabled()) // no thank you on the autorepeat
		{
			GetKeyboardDevice().OnKeyPressed(static_cast<unsigned char>(wparam));
		}
	}
	else if (msg == WM_KEYUP)
		GetKeyboardDevice().OnKeyReleased(static_cast<unsigned char>(wparam));
	else if (msg == WM_CHAR)
		GetKeyboardDevice().OnChar(static_cast<unsigned char>(wparam));
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
				GetMouseDevice().OnLeftReleased(pt.x, pt.y);
				GetMouseDevice().OnRightReleased(pt.x, pt.y);
			}
		}
	}
	else if (msg == WM_LBUTTONDOWN)
	{
		const POINTS pt = MAKEPOINTS(lparam);
		GetMouseDevice().OnLeftPressed(pt.x, pt.y);
		SetForegroundWindow(hwnd);
	}
	else if (msg == WM_RBUTTONDOWN)
	{
		const POINTS pt = MAKEPOINTS(lparam);
		GetMouseDevice().OnRightPressed(pt.x, pt.y);
	}
	else if (msg == WM_LBUTTONUP)
	{
		const POINTS pt = MAKEPOINTS(lparam);
		GetMouseDevice().OnLeftReleased(pt.x, pt.y);
	}
	else if (msg == WM_RBUTTONUP)
	{
		const POINTS pt = MAKEPOINTS(lparam);
		GetMouseDevice().OnRightReleased(pt.x, pt.y);
	}
	else if (msg == WM_MOUSEWHEEL)
	{
		const MOUSEHOOKSTRUCTEX* mhs = (const MOUSEHOOKSTRUCTEX*)lparam;
		short delta = HIWORD(mhs->mouseData);
		const POINTS pt = MAKEPOINTS(lparam);
		if (delta > 0)
		{
			GetMouseDevice().OnWheelUp(pt.x, pt.y, delta);
		}
		else if (delta < 0)
		{
			GetMouseDevice().OnWheelDown(pt.x, pt.y, delta);
		}
	}
	// ************ END MOUSE MESSAGES ************ //
}

InputManager::InputManager()
= default;
