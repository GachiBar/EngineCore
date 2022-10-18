#include "Mouse.h"
#include "InputKeys.h"
#include "InputManager.h"

std::pair<int,int> Mouse::GetPos() const
{
	return { x,y };
}

int Mouse::GetPosX() const
{
	return x;
}

int Mouse::GetPosY() const
{
	return y;
}

float Mouse::GetDelta() const
{
	return _delta;
}

bool Mouse::IsPressedDown(MouseKey key) const
{
	return std::ranges::any_of(last_changes, [key](const std::shared_ptr<InputEvent>& input_event)
	{
		return input_event->GetEventType() == EventType::MouseButtonPressed && 
			dynamic_cast<MouseButtonPressedEvent*>(input_event.get())->GetMouseButton() == key;
	});
}

bool Mouse::IsPressedUp(MouseKey key) const
{
	return std::ranges::any_of(last_changes, [key](const std::shared_ptr<InputEvent>& input_event)
		{
			return input_event->GetEventType() == EventType::MouseButtonReleased &&
				dynamic_cast<MouseButtonPressedEvent*>(input_event.get())->GetMouseButton() == key;
		});
}

bool Mouse::IsKeyPressed(MouseKey key) const
{
	return IsKeyPressed(static_cast<unsigned char>(key));
}

bool Mouse::IsKeyPressed(const unsigned char keycode) const
{
	return keystates[keycode];
}

bool Mouse::IsLeftPressed() const
{
	return IsKeyPressed(MouseKey::LButton);
}

bool Mouse::IsRightPressed() const
{
	return IsKeyPressed(MouseKey::RButton);
}

bool Mouse::IsMiddlePressed() const
{
	return IsKeyPressed(MouseKey::MButton);
}

bool Mouse::IsInWindow() const
{
	return isInWindow;
}

void Mouse::OnMouseLeave()
{
	isInWindow = false;
}

void Mouse::OnMouseEnter()
{
	isInWindow = true;
}

void Mouse::OnMouseMove(const int newx, const int newy )
{
	x = newx;
	y = newy;

	const std::shared_ptr<InputEvent> mouse_event(new MouseMovedEvent(static_cast<float>(newx), static_cast<float>(newy)));

	last_changes.push_back(mouse_event);
	InputManager::getInstance().SendEventToBuffer(mouse_event);
}

void Mouse::OnLeftPressed()
{
	keystates[static_cast<unsigned char>(MouseKey::LButton)] = true;

	const std::shared_ptr<InputEvent> mouse_event(new MouseButtonPressedEvent(MouseKey::LButton));

	last_changes.push_back(mouse_event);
	InputManager::getInstance().SendEventToBuffer(mouse_event);
}

void Mouse::OnLeftReleased()
{
	keystates[static_cast<unsigned char>(MouseKey::LButton)] = false;

	const std::shared_ptr<InputEvent> mouse_event(new MouseButtonReleasedEvent(MouseKey::LButton));

	last_changes.push_back(mouse_event);
	InputManager::getInstance().SendEventToBuffer(mouse_event);
}

void Mouse::OnRightPressed()
{
	keystates[static_cast<unsigned char>(MouseKey::RButton)] = true;

	const std::shared_ptr<InputEvent> mouse_event(new MouseButtonPressedEvent(MouseKey::RButton));

	last_changes.push_back(mouse_event);
	InputManager::getInstance().SendEventToBuffer(mouse_event);
}

void Mouse::OnRightReleased()
{
	keystates[static_cast<unsigned char>(MouseKey::RButton)] = false;

	const std::shared_ptr<InputEvent> mouse_event(new MouseButtonReleasedEvent(MouseKey::RButton));

	last_changes.push_back(mouse_event);
	InputManager::getInstance().SendEventToBuffer(mouse_event);
}

void Mouse::OnMiddlePressed()
{
	keystates[static_cast<unsigned char>(MouseKey::MButton)] = true;

	const std::shared_ptr<InputEvent> mouse_event(new MouseButtonPressedEvent(MouseKey::MButton));

	last_changes.push_back(mouse_event);
	InputManager::getInstance().SendEventToBuffer(mouse_event);
}

void Mouse::OnMiddleReleased()
{
	keystates[static_cast<unsigned char>(MouseKey::MButton)] = false;

	const std::shared_ptr<InputEvent> mouse_event(new MouseButtonReleasedEvent(MouseKey::MButton));

	last_changes.push_back(mouse_event);
	InputManager::getInstance().SendEventToBuffer(mouse_event);
}

void Mouse::OnWheelUp(short delta)
{
	const std::shared_ptr<InputEvent> mouse_event(new MouseScrolledEvent(delta));
	_delta = delta;

	last_changes.push_back(mouse_event);
	InputManager::getInstance().SendEventToBuffer(mouse_event);
}

void Mouse::OnWheelDown(short delta)
{
	const std::shared_ptr<InputEvent> mouse_event(new MouseScrolledEvent(delta));
	_delta = delta;

	last_changes.push_back(mouse_event);
	InputManager::getInstance().SendEventToBuffer(mouse_event);
}

void Mouse::Flush()
{
	_delta = 0.f;
	last_changes.clear();
}
