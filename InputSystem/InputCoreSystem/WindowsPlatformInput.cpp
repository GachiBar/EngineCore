#include "WindowsPlatformInput.h"
#include "Windows.h"
#include <cassert>
#include <map>

uint32 FWindowsPlatformInput::GetKeyMap( uint32* KeyCodes, std::string* KeyNames, uint32 MaxMappings )
{
#define ADDKEYMAP(KeyCode, KeyName)		if (NumMappings<MaxMappings) { KeyCodes[NumMappings]=KeyCode; KeyNames[NumMappings]=KeyName; ++NumMappings; };

	uint32 NumMappings = 0;

	if ( KeyCodes && KeyNames && (MaxMappings > 0) )
	{
		ADDKEYMAP( VK_LBUTTON, ("LeftMouseButton") );
		ADDKEYMAP( VK_RBUTTON, ("RightMouseButton") );
		ADDKEYMAP( VK_MBUTTON, ("MiddleMouseButton") );

		ADDKEYMAP( VK_XBUTTON1, ("ThumbMouseButton") );
		ADDKEYMAP( VK_XBUTTON2, ("ThumbMouseButton2") );

		ADDKEYMAP( VK_BACK, ("BackSpace") );
		ADDKEYMAP( VK_TAB, ("Tab") );
		ADDKEYMAP( VK_RETURN, ("Enter") );
		ADDKEYMAP( VK_PAUSE, ("Pause") );

		ADDKEYMAP( VK_CAPITAL, ("CapsLock") );
		ADDKEYMAP( VK_ESCAPE, ("Escape") );
		ADDKEYMAP( VK_SPACE, ("SpaceBar") );
		ADDKEYMAP( VK_PRIOR, ("PageUp") );
		ADDKEYMAP( VK_NEXT, ("PageDown") );
		ADDKEYMAP( VK_END, ("End") );
		ADDKEYMAP( VK_HOME, ("Home") );

		ADDKEYMAP( VK_LEFT, ("Left") );
		ADDKEYMAP( VK_UP, ("Up") );
		ADDKEYMAP( VK_RIGHT, ("Right") );
		ADDKEYMAP( VK_DOWN, ("Down") );

		ADDKEYMAP( VK_INSERT, ("Insert") );
		ADDKEYMAP( VK_DELETE, ("Delete") );

		ADDKEYMAP( VK_NUMPAD0, ("NumPadZero") );
		ADDKEYMAP( VK_NUMPAD1, ("NumPadOne") );
		ADDKEYMAP( VK_NUMPAD2, ("NumPadTwo") );
		ADDKEYMAP( VK_NUMPAD3, ("NumPadThree") );
		ADDKEYMAP( VK_NUMPAD4, ("NumPadFour") );
		ADDKEYMAP( VK_NUMPAD5, ("NumPadFive") );
		ADDKEYMAP( VK_NUMPAD6, ("NumPadSix") );
		ADDKEYMAP( VK_NUMPAD7, ("NumPadSeven") );
		ADDKEYMAP( VK_NUMPAD8, ("NumPadEight") );
		ADDKEYMAP( VK_NUMPAD9, ("NumPadNine") );

		ADDKEYMAP( VK_MULTIPLY, ("Multiply") );
		ADDKEYMAP( VK_ADD, ("Add") );
		ADDKEYMAP( VK_SUBTRACT, ("Subtract") );
		ADDKEYMAP( VK_DECIMAL, ("Decimal") );
		ADDKEYMAP( VK_DIVIDE, ("Divide") );

		ADDKEYMAP( VK_F1, ("F1") );
		ADDKEYMAP( VK_F2, ("F2") );
		ADDKEYMAP( VK_F3, ("F3") );
		ADDKEYMAP( VK_F4, ("F4") );
		ADDKEYMAP( VK_F5, ("F5") );
		ADDKEYMAP( VK_F6, ("F6") );
		ADDKEYMAP( VK_F7, ("F7") );
		ADDKEYMAP( VK_F8, ("F8") );
		ADDKEYMAP( VK_F9, ("F9") );
		ADDKEYMAP( VK_F10, ("F10") );
		ADDKEYMAP( VK_F11, ("F11") );
		ADDKEYMAP( VK_F12, ("F12") );

		ADDKEYMAP( VK_NUMLOCK, ("NumLock") );

		ADDKEYMAP( VK_SCROLL, ("ScrollLock") );

		ADDKEYMAP( VK_LSHIFT, ("LeftShift") );
		ADDKEYMAP( VK_RSHIFT, ("RightShift") );
		ADDKEYMAP( VK_LCONTROL, ("LeftControl") );
		ADDKEYMAP( VK_RCONTROL, ("RightControl") );
		ADDKEYMAP( VK_LMENU, ("LeftAlt") );
		ADDKEYMAP( VK_RMENU, ("RightAlt") );
		ADDKEYMAP( VK_LWIN, ("LeftCommand") );
		ADDKEYMAP( VK_RWIN, ("RightCommand") );

		std::map<uint32, uint32> ScanToVKMap;
#define MAP_OEM_VK_TO_SCAN(KeyCode) { const uint32 CharCode = MapVirtualKey(KeyCode,2); if (CharCode != 0) { ScanToVKMap[CharCode]=KeyCode; } }
		MAP_OEM_VK_TO_SCAN(VK_OEM_1);
		MAP_OEM_VK_TO_SCAN(VK_OEM_2);
		MAP_OEM_VK_TO_SCAN(VK_OEM_3);
		MAP_OEM_VK_TO_SCAN(VK_OEM_4);
		MAP_OEM_VK_TO_SCAN(VK_OEM_5);
		MAP_OEM_VK_TO_SCAN(VK_OEM_6);
		MAP_OEM_VK_TO_SCAN(VK_OEM_7);
		MAP_OEM_VK_TO_SCAN(VK_OEM_8);
		MAP_OEM_VK_TO_SCAN(VK_OEM_PLUS);
		MAP_OEM_VK_TO_SCAN(VK_OEM_COMMA);
		MAP_OEM_VK_TO_SCAN(VK_OEM_MINUS);
		MAP_OEM_VK_TO_SCAN(VK_OEM_PERIOD);
		MAP_OEM_VK_TO_SCAN(VK_OEM_102);
#undef  MAP_OEM_VK_TO_SCAN

		static const uint32 MAX_KEY_MAPPINGS(256);
		uint32 CharCodes[MAX_KEY_MAPPINGS];
		std::string CharKeyNames[MAX_KEY_MAPPINGS];
		const int32 CharMappings = GetCharKeyMap(CharCodes, CharKeyNames, MAX_KEY_MAPPINGS);

		for (int32 MappingIndex = 0; MappingIndex < CharMappings; ++MappingIndex)
		{
			ScanToVKMap.erase(CharCodes[MappingIndex]);
		}

        for(auto& it : ScanToVKMap)
        {
            ADDKEYMAP(it.second, char(it.first));
        }
	}

	assert(NumMappings < MaxMappings);
	return NumMappings;

#undef ADDKEYMAP
}

uint32 FWindowsPlatformInput::GetCharKeyMap(uint32* KeyCodes, std::string* KeyNames, uint32 MaxMappings)
{
	return GenericPlatformInput::GetStandardPrintableKeyMap(KeyCodes, KeyNames, MaxMappings, true, false);
}