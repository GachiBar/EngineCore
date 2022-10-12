#include "Keyboard.h"

#include "InputEvent.h"
#include <memory>

bool Keyboard::KeyIsPressed( unsigned char keycode ) const
{
	return keystates[keycode];
}

std::shared_ptr<KeyboardEvent> Keyboard::Read()
{
	if(!keybuffer.empty())
	{
		auto event_keyboard = keybuffer.front();
		keybuffer.pop();
		return event_keyboard;
	}
	return nullptr;
}

bool Keyboard::KeyIsEmpty() const
{
	return keybuffer.empty();
}

char Keyboard::ReadChar()
{
	if(!charbuffer.empty())
	{
		const unsigned char charcode = charbuffer.front();
		charbuffer.pop();
		return charcode;
	}
	return 0;
}

bool Keyboard::CharIsEmpty() const
{
	return charbuffer.empty();
}

void Keyboard::FlushKey()
{
	keybuffer = std::queue<std::shared_ptr<KeyboardEvent>>();
}

void Keyboard::FlushChar()
{
	charbuffer = std::queue<char>();
}

void Keyboard::Flush()
{
	FlushKey();
	FlushChar();
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
	keybuffer.push(et);
	TrimBuffer( keybuffer );
}

void Keyboard::OnKeyReleased( unsigned char keycode )
{
	keystates[ keycode ] = false;
	const auto virtual_key = static_cast<KeyboardKey>(keycode);
	const std::shared_ptr<KeyboardEvent> et(new KeyReleasedEvent(virtual_key));
	keybuffer.push(et);
	TrimBuffer( keybuffer );
}

void Keyboard::OnChar( char character )
{
	charbuffer.push( character );
	TrimBuffer( charbuffer );
}

template<typename T>
void Keyboard::TrimBuffer( std::queue<T>& buffer )
{
	while( buffer.size() > bufferSize )
	{
		buffer.pop();
	}
}

