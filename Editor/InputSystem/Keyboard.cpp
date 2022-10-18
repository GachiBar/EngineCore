#include "Keyboard.h"

#include "InputKeys.h"
#include <memory>

#include "InputManager.h"

bool Keyboard::IsKeyDown(KeyboardKey key) const
{
	return std::ranges::any_of(last_changes, [key](const std::shared_ptr<InputEvent>& input_event)
		{
			return input_event->GetEventType() == EventType::KeyPressed &&
				dynamic_cast<KeyPressedEvent*>(input_event.get())->GetKeyCode() == key;
		});
}

bool Keyboard::IsKeyUp(KeyboardKey key) const
{
	return std::ranges::any_of(last_changes, [key](const std::shared_ptr<InputEvent>& input_event)
		{
			return input_event->GetEventType() == EventType::KeyReleased &&
				dynamic_cast<KeyPressedEvent*>(input_event.get())->GetKeyCode() == key;
		});
}

bool Keyboard::KeyIsPressed( unsigned char keycode ) const
{
	return keystates[keycode];
}

void Keyboard::EnableAutorepeat()
{
	autorepeatEnabled = true;
}

void Keyboard::DisableAutorepeat()
{
	autorepeatEnabled = false;
}

bool Keyboard::IsAutorepeatEnabled() const
{
	return autorepeatEnabled;
}

void Keyboard::ClearState()
{
	keystates.reset();
}

void Keyboard::OnKeyPressed( unsigned char keycode )
{
	keystates[ keycode ] = true;
	const auto virtual_key = static_cast<KeyboardKey>(keycode);
	const std::shared_ptr<KeyboardEvent> et(new KeyPressedEvent(virtual_key));

	last_changes.push_back(et);
	InputManager::getInstance().SendEventToBuffer(et);
}

void Keyboard::OnKeyReleased( unsigned char keycode )
{
	keystates[ keycode ] = false;
	const auto virtual_key = static_cast<KeyboardKey>(keycode);
	const std::shared_ptr<KeyboardEvent> et(new KeyReleasedEvent(virtual_key));

	last_changes.push_back(et);
	InputManager::getInstance().SendEventToBuffer(et);
}

void Keyboard::Flush()
{
	last_changes.clear();
}
