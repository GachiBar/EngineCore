#pragma once
#include <memory>
#include <queue>

class InputEvent;

class Mouse
{
public:

	Mouse() = default;
	Mouse( const Mouse& ) = delete;
	Mouse& operator=( const Mouse& ) = delete;
	std::pair<int,int> GetPos() const;
	int GetPosX() const;
	int GetPosY() const;
	bool LeftIsPressed() const;
	bool RightIsPressed() const;
	bool IsInWindow() const;
	std::shared_ptr<InputEvent> Read();
	bool IsEmpty() const
	{
		return buffer.empty();
	}
	void Flush();

	void OnMouseMove(int x, int y);
	void OnMouseLeave();
	void OnMouseEnter();

	void OnLeftPressed(int x, int y);
	void OnLeftReleased(int x, int y);

	void OnRightPressed(int x, int y);
	void OnRightReleased(int x, int y);

	void OnMiddlePressed(int x, int y);
	void OnMiddleReleased(int x, int y);


	void OnWheelUp(int x, int y, short delta);
	void OnWheelDown(int x, int y, short delta);
private:

	void TrimBuffer();
private:
	static constexpr unsigned int bufferSize = 4u;
	int x;
	int y;
	bool leftIsPressed = false;
	bool rightIsPressed = false;
	bool middleIsPressed = false;
	bool isInWindow = false;
	std::queue<std::shared_ptr<InputEvent>> buffer;
};