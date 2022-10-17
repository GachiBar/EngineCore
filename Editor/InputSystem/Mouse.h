#pragma once
#include <bitset>
#include <memory>
#include <queue>

#include "InputEvent.h"

class InputEvent;

class Mouse
{
	friend class InputManager;
public:

	Mouse() = default;
	Mouse( const Mouse& ) = delete;
	Mouse& operator=( const Mouse& ) = delete;

	std::pair<int,int> GetPos() const;
	int GetPosX() const;
	int GetPosY() const;

	float GetDelta() const;

	bool IsPressedDown(MouseKey key) const;
	bool IsPressedUp(MouseKey key) const;
	
	bool IsKeyPressed(MouseKey key) const;
	bool IsKeyPressed(unsigned char keycode) const;

	bool IsLeftPressed() const;
	bool IsRightPressed() const;
	bool IsMiddlePressed() const;

	bool IsInWindow() const;
protected:
	void OnMouseMove(int newx, int newy);
	void OnMouseLeave();
	void OnMouseEnter();

	void OnLeftPressed();
	void OnLeftReleased();

	void OnRightPressed();
	void OnRightReleased();

	void OnMiddlePressed();
	void OnMiddleReleased();

	void OnWheelUp(short delta);
	void OnWheelDown(short delta);

	void Flush();

protected:
	int x;
	int y;
	float _delta;

	bool isInWindow = false;

	static constexpr unsigned int nKeys = 4u;
	std::bitset<nKeys> keystates;

	std::vector<std::shared_ptr<InputEvent>> last_changes;
};