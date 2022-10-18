#pragma once
#include "InputKeys.h"
//#include "../libs/nameof.hpp"

#define BIT(x) (1 << x)



enum class EventType
{
	None = 0,
	WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
	KeyPressed, KeyReleased, KeyTyped,
	MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

enum EventCategory
{
	None = 0,
	EventCategoryApplication = BIT(0),
	EventCategoryInput = BIT(1),
	EventCategoryKeyboard = BIT(2),
	EventCategoryMouse = BIT(3),
	EventCategoryMouseButton = BIT(4)
};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }


class InputEvent
{
public:
	virtual ~InputEvent() = default;

	bool Handled = false;

	virtual EventType GetEventType() const = 0;
	virtual const char* GetName() const = 0;
	virtual int GetCategoryFlags() const = 0;

	bool IsInCategory(EventCategory category) const
	{
		return GetCategoryFlags() & category;
	}
};

class MouseMovedEvent : public InputEvent
{
public:
	MouseMovedEvent(const float x, const float y)
		: m_MouseX(x), m_MouseY(y) {}

	float GetX() const { return m_MouseX; }
	float GetY() const { return m_MouseY; }

	EVENT_CLASS_TYPE(MouseMoved)
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
private:
	float m_MouseX, m_MouseY;
};

class MouseScrolledEvent : public InputEvent
{
public:
	MouseScrolledEvent(const float delta)
		:m_delta(delta) {}

	float GetDelta()
	{
		return m_delta;
	}

	EVENT_CLASS_TYPE(MouseScrolled)
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
private:
	float m_delta;
};

class MouseButtonEvent : public InputEvent
{
public:
	MouseKey GetMouseButton() const { return m_Button; }

	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)
protected:
	MouseButtonEvent(const MouseKey button)
		: m_Button(button) {}

	MouseKey m_Button;
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
	MouseButtonPressedEvent(const MouseKey button)
		: MouseButtonEvent(button) {}

	EVENT_CLASS_TYPE(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
	MouseButtonReleasedEvent(const MouseKey button)
		: MouseButtonEvent(button) {}
	
	EVENT_CLASS_TYPE(MouseButtonReleased)
};

class KeyboardEvent : public InputEvent
{
public:
	KeyboardKey GetKeyCode() const { return m_KeyCode; }

	EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
protected:
	KeyboardEvent(const KeyboardKey keycode)
		: m_KeyCode(keycode) {}

	KeyboardKey m_KeyCode;
};

class KeyPressedEvent : public KeyboardEvent
{
public:
	KeyPressedEvent(const KeyboardKey keycode, bool isRepeat = false)
		: KeyboardEvent(keycode), m_IsRepeat(isRepeat) {}

	bool IsRepeat() const { return m_IsRepeat; }

	EVENT_CLASS_TYPE(KeyPressed)
private:
	bool m_IsRepeat;
};

class KeyReleasedEvent : public KeyboardEvent
{
public:
	KeyReleasedEvent(const KeyboardKey keycode)
		: KeyboardEvent(keycode) {}

	EVENT_CLASS_TYPE(KeyReleased)
};

class KeyTypedEvent : public KeyboardEvent
{
public:
	KeyTypedEvent(const KeyboardKey keycode)
		: KeyboardEvent(keycode)
	{
		auto t = KeyboardKey::A;
		//volatile auto t2 = NAMEOF_ENUM(t);
	}

	EVENT_CLASS_TYPE(KeyTyped)
};

#undef EVENT_CLASS_TYPE
#undef EVENT_CLASS_CATEGORY
#undef BIT