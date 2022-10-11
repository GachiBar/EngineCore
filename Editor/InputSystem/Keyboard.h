#pragma once
#include <queue>
#include <bitset>
#include <memory>

class KeyboardEvent;

class Keyboard
{
public:
	Keyboard() = default;
	Keyboard( const Keyboard& ) = delete;
	Keyboard& operator=( const Keyboard& ) = delete;
	bool KeyIsPressed( unsigned char keycode ) const;
	std::shared_ptr<KeyboardEvent> ReadKey();
	bool KeyIsEmpty() const;
	char ReadChar();
	bool CharIsEmpty() const;
	void FlushKey();
	void FlushChar();
	void Flush();
	void EnableAutorepeat();
	void DisableAutorepeat();
	bool IsAutorepeatEnabled() const;
	void ClearState();
	void OnKeyPressed(unsigned char keycode);
	void OnKeyReleased(unsigned char keycode);
	void OnChar(char character);

private:

	template<typename T>
	void TrimBuffer( std::queue<T>& buffer );
private:
	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int bufferSize = 4u;
	bool autorepeatEnabled = false;
	std::bitset<nKeys> keystates;
	std::queue<std::shared_ptr<KeyboardEvent>> keybuffer;
	std::queue<char> charbuffer;
};
