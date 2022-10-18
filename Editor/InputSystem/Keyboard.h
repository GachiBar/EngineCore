#pragma once
#include <queue>
#include <bitset>
#include <memory>

enum class KeyboardKey;
class InputEvent;

class Keyboard
{
	friend class InputManager;
public:
	Keyboard() = default;
	Keyboard( const Keyboard& ) = delete;
	Keyboard& operator=( const Keyboard& ) = delete;

	bool IsKeyDown(KeyboardKey key) const;
	bool IsKeyUp(KeyboardKey key) const;
	bool KeyIsPressed( unsigned char keycode ) const;

	void EnableAutorepeat();
	void DisableAutorepeat();
	bool IsAutorepeatEnabled() const;
	void ClearState();

protected:
	void OnKeyPressed(unsigned char keycode);
	void OnKeyReleased(unsigned char keycode);

	void Flush();
protected:
	static constexpr unsigned int nKeys = 256u;
	bool autorepeatEnabled = false;
	std::bitset<nKeys> keystates;

	std::vector<std::shared_ptr<InputEvent>> last_changes;
};
