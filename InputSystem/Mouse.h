#pragma once
#include <set>

#include "InputCoreSystem/KeyStruct.h"
#include "../Core/Definitions.h"

class FGenericWindow;
class InputEvent;

class Mouse
{
	friend class InputManager;
public:

	Mouse() = default;
	Mouse( const Mouse& ) = delete;
	Mouse& operator=( const Mouse& ) = delete;

	void SetPos(int x, int y);
	std::pair<int, int> GetPos() const;
	FVector2D GetCurPosFloatPoint() const;

	void SetLastPos(int x, int y);
	std::pair<int, int> GetLastPos() const;
	FVector2D GetLastPosFloatPoint() const;

	bool IsInWindow(const FGenericWindow* Window) const;


	void AddPressedMouseButton(FKey const & MouseButton);
	void RemoveMouseButton(FKey const& MouseButton);
	std::set<FKey> const& GetPressedButtons();

	void Flush();

protected:
	std::set<FKey> PressedMouseButtons;

	std::pair<int, int> last_pos;
	std::pair<int, int> cur_pos;

};