#pragma once
#include <string>
#include "KeyStruct.h"
#include "../../Core/Definitions.h"

struct GenericPlatformInput
{
public:
	static uint32 GetKeyMap( uint32* KeyCodes, std::string* KeyNames, uint32 MaxMappings )
	{
		return 0;
	}

	static uint32 GetCharKeyMap(uint32* KeyCodes, std::string* KeyNames, uint32 MaxMappings)
	{
		return 0;
	}

	/** Remap a given key to something else if necessary. */
	static FKey RemapKey(FKey Key) { return Key; }

protected:
	/**
	* Retrieves some standard key code mappings (usually called by a subclass's GetCharKeyMap)
	*
	* @param OutKeyMap Key map to add to.
	* @param bMapUppercaseKeys If true, will map A, B, C, etc to EKeys::A, EKeys::B, EKeys::C
	* @param bMapLowercaseKeys If true, will map a, b, c, etc to EKeys::A, EKeys::B, EKeys::C
	*/
	static uint32 GetStandardPrintableKeyMap(uint32* KeyCodes, std::string* KeyNames, uint32 MaxMappings, bool bMapUppercaseKeys, bool bMapLowercaseKeys);
};
