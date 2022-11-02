#include "Mouse.h"
#include "InputManager.h"

void Mouse::SetPos(int x, int y)
{
	cur_pos.first = x;
	cur_pos.second = y;
}

std::pair<int,int> Mouse::GetPos() const
{
	return cur_pos;
}

FVector2D Mouse::GetCurPosFloatPoint() const
{
	return FVector2D(cur_pos.first, cur_pos.second);
}

void Mouse::SetLastPos(int x, int y)
{
	last_pos.first = x;
	last_pos.second = y;
}

std::pair<int, int> Mouse::GetLastPos() const
{
	return last_pos;
}

FVector2D Mouse::GetLastPosFloatPoint() const
{
	return FVector2D(last_pos.first, last_pos.second);
}

bool Mouse::IsInWindow(const FGenericWindow* Window) const
{
	return Window->IsPointInWindow(cur_pos.first, cur_pos.second);
}

void Mouse::AddPressedMouseButton(FKey const& MouseButton)
{
	if (MouseButton.IsMouseButton())
		PressedMouseButtons.insert(MouseButton);
}

void Mouse::RemoveMouseButton(FKey const& MouseButton)
{
	if (MouseButton.IsMouseButton())
		PressedMouseButtons.erase(MouseButton);
}

std::set<FKey> const& Mouse::GetPressedButtons()
{
	return PressedMouseButtons;
}

void Mouse::Flush()
{
	InputManager::getInstance().GetPlayerInput()->SetKeyStateValue(EKeys::MouseWheelAxis, 0.f);
	InputManager::getInstance().GetPlayerInput()->SetKeyStateValue(EKeys::MouseX, 0.f);
	InputManager::getInstance().GetPlayerInput()->SetKeyStateValue(EKeys::MouseY, 0.f);

	PressedMouseButtons.clear();
}
