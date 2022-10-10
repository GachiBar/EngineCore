#include "InputManager.h"

InputManager& InputManager::getInstance()
{
	static InputManager instance;
	// Instantiated on first use.
	return instance;
}

Keyboard& InputManager::GetKeyboardDevice()
{
	return kbd;
}

Mouse& InputManager::GetMouseDevice()
{
	return mouse;
}

InputManager::InputManager()
= default;
