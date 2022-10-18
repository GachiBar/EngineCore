#include "InputManager.h"

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
	return mouse;
}

bool InputManager::IsInputMessage(UINT msg)
{
	return IsMouseMessage(msg) || IsKeyboardMessage(msg);
}

bool InputManager::IsMouseMessage(UINT msg)
{
	switch (msg)
	{
	case WM_MOUSEMOVE:
	case WM_MOUSELEAVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONDBLCLK:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONDBLCLK:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	case WM_XBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_MOUSEHWHEEL:
		return true;
	default:
		return false;
	}
}

bool InputManager::IsKeyboardMessage(UINT msg)
{
	switch (msg)
	{
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
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
	else if (msg == WM_KEYDOWN)
	{
		if (!(lparam & 0x40000000) || GetKeyboardDevice().IsAutorepeatEnabled()) // no thank you on the autorepeat
		{
			GetKeyboardDevice().OnKeyPressed(static_cast<unsigned char>(wparam));
		}
	}
	else if (msg == WM_KEYUP)
		GetKeyboardDevice().OnKeyReleased(static_cast<unsigned char>(wparam));
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
		
		if (delta > 0)
		{
			GetMouseDevice().OnWheelUp(copysignf(1.f, delta));
		}
		else if (delta < 0)
		{
			GetMouseDevice().OnWheelDown(copysignf(1.f, delta));
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