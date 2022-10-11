/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Mouse.cpp																			  *
 *	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
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
#include "Mouse.h"
#include "InputEvent.h"

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

bool Mouse::LeftIsPressed() const
{
	return leftIsPressed;
}

bool Mouse::RightIsPressed() const
{
	return rightIsPressed;
}

bool Mouse::IsInWindow() const
{
	return isInWindow;
}

std::shared_ptr<InputEvent> Mouse::Read()
{
	if( buffer.size() > 0u )
	{
		auto e = buffer.front();
		buffer.pop();
		return e;
	}
	return nullptr;
}

void Mouse::Flush()
{
	buffer = std::queue<std::shared_ptr<InputEvent>>();
}

void Mouse::OnMouseLeave()
{
	isInWindow = false;
}

void Mouse::OnMouseEnter()
{
	isInWindow = true;
}

void Mouse::OnMouseMove( int newx,int newy )
{
	x = newx;
	y = newy;

	const std::shared_ptr<InputEvent> mouse_event(new MouseMovedEvent(static_cast<float>(newx), static_cast<float>(newy)));

	buffer.push(mouse_event);
	TrimBuffer();
}

void Mouse::OnLeftPressed( int x,int y )
{
	leftIsPressed = true;

	const std::shared_ptr<InputEvent> mouse_event(new MouseButtonPressedEvent(MouseCode::LButton));

	buffer.push(mouse_event);
	TrimBuffer();
}

void Mouse::OnLeftReleased( int x,int y )
{
	leftIsPressed = false;

	const std::shared_ptr<InputEvent> mouse_event(new MouseButtonReleasedEvent(MouseCode::LButton));
	
	buffer.push(mouse_event);
	TrimBuffer();
}

void Mouse::OnRightPressed( int x,int y )
{
	rightIsPressed = true;

	const std::shared_ptr<InputEvent> mouse_event(new MouseButtonPressedEvent(MouseCode::RButton));

	buffer.push(mouse_event);
	TrimBuffer();
}

void Mouse::OnRightReleased( int x,int y )
{
	rightIsPressed = false;

	const std::shared_ptr<InputEvent> mouse_event(new MouseButtonReleasedEvent(MouseCode::RButton));

	buffer.push(mouse_event);
	TrimBuffer();
}

void Mouse::OnMiddlePressed(int x, int y)
{
	middleIsPressed = true;

	const std::shared_ptr<InputEvent> mouse_event(new MouseButtonPressedEvent(MouseCode::MButton));

	buffer.push(mouse_event);
	TrimBuffer();
}

void Mouse::OnMiddleReleased(int x, int y)
{
	middleIsPressed = false;

	const std::shared_ptr<InputEvent> mouse_event(new MouseButtonReleasedEvent(MouseCode::MButton));

	buffer.push(mouse_event);
	TrimBuffer();
}

void Mouse::OnWheelUp( int x,int y, short delta)
{
	const std::shared_ptr<InputEvent> mouse_event(new MouseScrolledEvent(delta));

	buffer.push(mouse_event);
	TrimBuffer();
}

void Mouse::OnWheelDown( int x,int y, short delta)
{
	const std::shared_ptr<InputEvent> mouse_event(new MouseScrolledEvent(delta));

	buffer.push(mouse_event);
	TrimBuffer();
}

void Mouse::TrimBuffer()
{
	while( buffer.size() > bufferSize )
	{
		buffer.pop();
	}
}