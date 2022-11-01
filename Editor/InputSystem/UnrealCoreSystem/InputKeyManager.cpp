#include "InputKeyManager.h"

#include <cassert>

#include "WindowsPlatformInput.h"
#include "KeyDetails.h"

std::shared_ptr<InputKeyManager> InputKeyManager::Instance;

/**
 * Returns the instance of the input key manager
 */
InputKeyManager& InputKeyManager::Get()
{
	if( !Instance )
	{
		Instance.reset( new InputKeyManager() );
	}
	return *Instance;
}

void InputKeyManager::InitKeyMappings()
{
	static const uint32 MAX_KEY_MAPPINGS(256);
	uint32 KeyCodes[MAX_KEY_MAPPINGS], CharCodes[MAX_KEY_MAPPINGS];
	std::string KeyNames[MAX_KEY_MAPPINGS], CharKeyNames[MAX_KEY_MAPPINGS];

	uint32 const CharKeyMapSize(FPlatformInput::GetCharKeyMap(CharCodes, CharKeyNames, MAX_KEY_MAPPINGS));
	uint32 const KeyMapSize(FPlatformInput::GetKeyMap(KeyCodes, KeyNames, MAX_KEY_MAPPINGS));

	// When the input language changes, a key that was virtual may no longer be virtual.
	// We must repopulate the maps to ensure GetKeyFromCodes returns the correct value per language.
	KeyMapVirtualToEnum.clear();
	KeyMapCharToEnum.clear();
	for (uint32 Idx=0; Idx<KeyMapSize; ++Idx)
	{
		FKey Key(KeyNames[Idx]);

		if (!Key.IsValid())
		{
			EKeys::AddKey(FKeyDetails(Key, Key.GetDisplayName()));
		}

		KeyMapVirtualToEnum[KeyCodes[Idx]]= Key;
	}

	for (uint32 Idx=0; Idx<CharKeyMapSize; ++Idx)
	{
		// repeated linear search here isn't ideal, but it's just once at startup
		const FKey Key(CharKeyNames[Idx]);

		//if (ensureMsgf(Key.IsValid(), TEXT("Failed to get key for name %s"), *CharKeyNames[Idx]))
        if(Key.IsValid())
		{
			KeyMapCharToEnum[CharCodes[Idx]] =  Key;
		}
	}
}

/**
 * Retrieves the key mapped to the specified key or character code.
 *
 * @param	KeyCode	the key code to get the name for
 */
FKey InputKeyManager::GetKeyFromCodes(const uint32 KeyCode, const uint32 CharCode, const bool IsChar) const
{
	FKey ResultKey = EKeys::Invalid;

	if (!IsChar && KeyMapVirtualToEnum.contains(KeyCode))
		ResultKey = KeyMapVirtualToEnum.at(KeyCode);

	if(ResultKey == EKeys::Invalid && KeyMapCharToEnum.contains(CharCode) && IsChar)
	{
		ResultKey = KeyMapCharToEnum.at(CharCode);
	}

    // If we didn't find a FKey and the CharCode is not a control character (using 32/space as the start of that range),
	// then we want to synthesize a new FKey for this unknown character so that key binding on non-qwerty keyboards works better
    if(ResultKey == EKeys::Invalid && CharCode > 32)
    {
        //FKey NewKey(FKey::SyntheticCharPrefix + std::to_string(CharCode));
		//EKeys::AddKey(FKeyDetails(NewKey, std::string(1,char(CharCode)), FKeyDetails::NotActionBindableKey));
		//const_cast<InputKeyManager*>(this)->KeyMapCharToEnum[CharCode] = NewKey;
		//return NewKey;
	}

	return ResultKey;
}

void InputKeyManager::GetCodesFromKey(const FKey Key, const uint32*& KeyCode, const uint32*& CharCode) const
{
	assert(true);
	auto find_key = [&](std::map<uint32,FKey>& MapToCheck, const FKey & KeyToCheck) ->uint32 {
		const auto find_result = std::ranges::find_if(MapToCheck, [&](const std::pair<uint32, FKey>& pair)
		{
			return pair.second == Key;
		});


		if (find_result != std::end(MapToCheck))
		{
			return find_result->first;
		}
		assert(true);
	};
	
	//CharCode = find_key(KeyMapCharToEnum,Key);
	//KeyCode = KeyMapVirtualToEnum.FindKey(Key);
}