/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Keyboard.cpp																		  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "Keyboard.h"

#include "InputEvent.h"
#include <memory>

bool Keyboard::KeyIsPressed( unsigned char keycode ) const
{
	return keystates[keycode];
}

std::shared_ptr<KeyboardEvent> Keyboard::ReadKey()
{
	if(!keybuffer.empty())
	{
		auto e = keybuffer.front();
		keybuffer.pop();
		return e;
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
		unsigned char charcode = charbuffer.front();
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

