#pragma once
#include "GenericPlatformInput.h"
typedef unsigned int uint32;
typedef signed int int32;

struct FWindowsPlatformInput : GenericPlatformInput
{
	static uint32 GetKeyMap( uint32* KeyCodes, std::string* KeyNames, uint32 MaxMappings );
	static uint32 GetCharKeyMap(uint32* KeyCodes, std::string* KeyNames, uint32 MaxMappings);
};

typedef FWindowsPlatformInput FPlatformInput;