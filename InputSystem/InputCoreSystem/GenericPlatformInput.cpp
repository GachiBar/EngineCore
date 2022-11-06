#include "GenericPlatformInput.h"

uint32 GenericPlatformInput::GetStandardPrintableKeyMap(uint32* KeyCodes, std::string* KeyNames, uint32 MaxMappings, bool bMapUppercaseKeys, bool bMapLowercaseKeys)
{
	uint32 NumMappings = 0;

#define ADDKEYMAP(KeyCode, KeyName)		if (NumMappings<MaxMappings) { KeyCodes[NumMappings]=KeyCode; KeyNames[NumMappings]=KeyName; ++NumMappings; };

	ADDKEYMAP( '0', ("Zero") );
	ADDKEYMAP( '1', ("One") );
	ADDKEYMAP( '2', ("Two") );
	ADDKEYMAP( '3', ("Three") );
	ADDKEYMAP( '4', ("Four") );
	ADDKEYMAP( '5', ("Five") );
	ADDKEYMAP( '6', ("Six") );
	ADDKEYMAP( '7', ("Seven") );
	ADDKEYMAP( '8', ("Eight") );
	ADDKEYMAP( '9', ("Nine") );

	// we map both upper and lower
	if (bMapUppercaseKeys)
	{
		ADDKEYMAP( 'A', ("A") );
		ADDKEYMAP( 'B', ("B") );
		ADDKEYMAP( 'C', ("C") );
		ADDKEYMAP( 'D', ("D") );
		ADDKEYMAP( 'E', ("E") );
		ADDKEYMAP( 'F', ("F") );
		ADDKEYMAP( 'G', ("G") );
		ADDKEYMAP( 'H', ("H") );
		ADDKEYMAP( 'I', ("I") );
		ADDKEYMAP( 'J', ("J") );
		ADDKEYMAP( 'K', ("K") );
		ADDKEYMAP( 'L', ("L") );
		ADDKEYMAP( 'M', ("M") );
		ADDKEYMAP( 'N', ("N") );
		ADDKEYMAP( 'O', ("O") );
		ADDKEYMAP( 'P', ("P") );
		ADDKEYMAP( 'Q', ("Q") );
		ADDKEYMAP( 'R', ("R") );
		ADDKEYMAP( 'S', ("S") );
		ADDKEYMAP( 'T', ("T") );
		ADDKEYMAP( 'U', ("U") );
		ADDKEYMAP( 'V', ("V") );
		ADDKEYMAP( 'W', ("W") );
		ADDKEYMAP( 'X', ("X") );
		ADDKEYMAP( 'Y', ("Y") );
		ADDKEYMAP( 'Z', ("Z") );
	}

	if (bMapLowercaseKeys)
	{
		ADDKEYMAP( 'a', ("A") );
		ADDKEYMAP( 'b', ("B") );
		ADDKEYMAP( 'c', ("C") );
		ADDKEYMAP( 'd', ("D") );
		ADDKEYMAP( 'e', ("E") );
		ADDKEYMAP( 'f', ("F") );
		ADDKEYMAP( 'g', ("G") );
		ADDKEYMAP( 'h', ("H") );
		ADDKEYMAP( 'i', ("I") );
		ADDKEYMAP( 'j', ("J") );
		ADDKEYMAP( 'k', ("K") );
		ADDKEYMAP( 'l', ("L") );
		ADDKEYMAP( 'm', ("M") );
		ADDKEYMAP( 'n', ("N") );
		ADDKEYMAP( 'o', ("O") );
		ADDKEYMAP( 'p', ("P") );
		ADDKEYMAP( 'q', ("Q") );
		ADDKEYMAP( 'r', ("R") );
		ADDKEYMAP( 's', ("S") );
		ADDKEYMAP( 't', ("T") );
		ADDKEYMAP( 'u', ("U") );
		ADDKEYMAP( 'v', ("V") );
		ADDKEYMAP( 'w', ("W") );
		ADDKEYMAP( 'x', ("X") );
		ADDKEYMAP( 'y', ("Y") );
		ADDKEYMAP( 'z', ("Z") );
	}

	ADDKEYMAP( ';', ("Semicolon") );
	ADDKEYMAP( '=', ("Equals") );
	ADDKEYMAP( ',', ("Comma") );
	ADDKEYMAP( '-', ("Hyphen") );
	ADDKEYMAP( '.', ("Period") );
	ADDKEYMAP( '/', ("Slash") );
	ADDKEYMAP( '~', ("Tilde") );
	ADDKEYMAP( '[', ("LeftBracket") );
	ADDKEYMAP( '\\', ("Backslash") );
	ADDKEYMAP( ']', ("RightBracket") );
	ADDKEYMAP( '\'', ("Apostrophe") );
	ADDKEYMAP(' ', ("SpaceBar"));

	// AZERTY Keys
	ADDKEYMAP( '&', ("Ampersand") );
	ADDKEYMAP( '*', ("Asterix") );
	ADDKEYMAP( '^', ("Caret") );
	ADDKEYMAP( ':', ("Colon") );
	ADDKEYMAP( '$', ("Dollar") );
	ADDKEYMAP( '!', ("Exclamation") );
	ADDKEYMAP( '(', ("LeftParantheses") );
	ADDKEYMAP( ')', ("RightParantheses") );
	ADDKEYMAP( '"', ("Quote") );
	ADDKEYMAP( '_', ("Underscore") );
	ADDKEYMAP( 224, ("A_AccentGrave") );
	ADDKEYMAP( 231, ("C_Cedille") );
	ADDKEYMAP( 233, ("E_AccentAigu") );
	ADDKEYMAP( 232, ("E_AccentGrave") );
	ADDKEYMAP( 167, ("Section") );

#undef ADDKEYMAP

	return NumMappings;
}