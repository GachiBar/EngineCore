#pragma once
#include "KeyStruct.h"
#include <memory>
#include <map>

struct InputKeyManager
{
public:
	static InputKeyManager& Get();

	void GetCodesFromKey(const FKey Key, const uint32*& KeyCode, const uint32*& CharCode) const;

	/**
	 * Retrieves the key mapped to the specified character code.
	 * @param KeyCode	The key code to get the name for.
	 */
	FKey GetKeyFromCodes(const uint32 KeyCode, const uint32 CharCode) const;
	void InitKeyMappings();
private:
	InputKeyManager()
	{
		InitKeyMappings();
	}

	static std::shared_ptr<InputKeyManager > Instance;
	std::map<uint32, FKey> KeyMapVirtualToEnum;
	std::map<uint32, FKey> KeyMapCharToEnum;
};