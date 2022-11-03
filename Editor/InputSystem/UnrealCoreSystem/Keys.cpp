#include "Keys.h"

#include <algorithm>

#include "KeyStruct.h"
#include "KeyDetails.h"
#include <map>
#include <ranges>

#include "InputKeyManager.h"

static const std::string NAME_None = "";

const FKey EKeys::AnyKey("AnyKey");

const FKey EKeys::MouseX("MouseX");
const FKey EKeys::MouseY("MouseY");
const FKey EKeys::MouseScrollUp("MouseScrollUp");
const FKey EKeys::MouseScrollDown("MouseScrollDown");
const FKey EKeys::MouseWheelAxis("MouseWheelAxis");

const FKey EKeys::LeftMouseButton("LeftMouseButton");
const FKey EKeys::RightMouseButton("RightMouseButton");
const FKey EKeys::MiddleMouseButton("MiddleMouseButton");
const FKey EKeys::ThumbMouseButton("ThumbMouseButton");
const FKey EKeys::ThumbMouseButton2("ThumbMouseButton2");

const FKey EKeys::BackSpace("BackSpace");
const FKey EKeys::Tab("Tab");
const FKey EKeys::Enter("Enter");
const FKey EKeys::Pause("Pause");

const FKey EKeys::CapsLock("CapsLock");
const FKey EKeys::Escape("Escape");
const FKey EKeys::SpaceBar("SpaceBar");
const FKey EKeys::PageUp("PageUp");
const FKey EKeys::PageDown("PageDown");
const FKey EKeys::End("End");
const FKey EKeys::Home("Home");

const FKey EKeys::Left("Left");
const FKey EKeys::Up("Up");
const FKey EKeys::Right("Right");
const FKey EKeys::Down("Down");

const FKey EKeys::Insert("Insert");
const FKey EKeys::Delete("Delete");

const FKey EKeys::Zero("Zero");
const FKey EKeys::One("One");
const FKey EKeys::Two("Two");
const FKey EKeys::Three("Three");
const FKey EKeys::Four("Four");
const FKey EKeys::Five("Five");
const FKey EKeys::Six("Six");
const FKey EKeys::Seven("Seven");
const FKey EKeys::Eight("Eight");
const FKey EKeys::Nine("Nine");

const FKey EKeys::A("A");
const FKey EKeys::B("B");
const FKey EKeys::C("C");
const FKey EKeys::D("D");
const FKey EKeys::E("E");
const FKey EKeys::F("F");
const FKey EKeys::G("G");
const FKey EKeys::H("H");
const FKey EKeys::I("I");
const FKey EKeys::J("J");
const FKey EKeys::K("K");
const FKey EKeys::L("L");
const FKey EKeys::M("M");
const FKey EKeys::N("N");
const FKey EKeys::O("O");
const FKey EKeys::P("P");
const FKey EKeys::Q("Q");
const FKey EKeys::R("R");
const FKey EKeys::S("S");
const FKey EKeys::T("T");
const FKey EKeys::U("U");
const FKey EKeys::V("V");
const FKey EKeys::W("W");
const FKey EKeys::X("X");
const FKey EKeys::Y("Y");
const FKey EKeys::Z("Z");

const FKey EKeys::NumPadZero("NumPadZero");
const FKey EKeys::NumPadOne("NumPadOne");
const FKey EKeys::NumPadTwo("NumPadTwo");
const FKey EKeys::NumPadThree("NumPadThree");
const FKey EKeys::NumPadFour("NumPadFour");
const FKey EKeys::NumPadFive("NumPadFive");
const FKey EKeys::NumPadSix("NumPadSix");
const FKey EKeys::NumPadSeven("NumPadSeven");
const FKey EKeys::NumPadEight("NumPadEight");
const FKey EKeys::NumPadNine("NumPadNine");

const FKey EKeys::Multiply("Multiply");
const FKey EKeys::Add("Add");
const FKey EKeys::Subtract("Subtract");
const FKey EKeys::Decimal("Decimal");
const FKey EKeys::Divide("Divide");

const FKey EKeys::F1("F1");
const FKey EKeys::F2("F2");
const FKey EKeys::F3("F3");
const FKey EKeys::F4("F4");
const FKey EKeys::F5("F5");
const FKey EKeys::F6("F6");
const FKey EKeys::F7("F7");
const FKey EKeys::F8("F8");
const FKey EKeys::F9("F9");
const FKey EKeys::F10("F10");
const FKey EKeys::F11("F11");
const FKey EKeys::F12("F12");

const FKey EKeys::NumLock("NumLock");

const FKey EKeys::ScrollLock("ScrollLock");

const FKey EKeys::LeftShift("LeftShift");
const FKey EKeys::RightShift("RightShift");
const FKey EKeys::LeftControl("LeftControl");
const FKey EKeys::RightControl("RightControl");
const FKey EKeys::LeftAlt("LeftAlt");
const FKey EKeys::RightAlt("RightAlt");
const FKey EKeys::LeftCommand("LeftCommand");
const FKey EKeys::RightCommand("RightCommand");

const FKey EKeys::Semicolon("Semicolon");
const FKey EKeys::Equals("Equals");
const FKey EKeys::Comma("Comma");
const FKey EKeys::Underscore("Underscore");
const FKey EKeys::Hyphen("Hyphen");
const FKey EKeys::Period("Period");
const FKey EKeys::Slash("Slash");
const FKey EKeys::Tilde("Tilde");
const FKey EKeys::LeftBracket("LeftBracket");
const FKey EKeys::LeftParantheses("LeftParantheses");
const FKey EKeys::Backslash("Backslash");
const FKey EKeys::RightBracket("RightBracket");
const FKey EKeys::RightParantheses("RightParantheses");
const FKey EKeys::Apostrophe("Apostrophe");
const FKey EKeys::Quote("Quote");

const FKey EKeys::Asterix("Asterix");
const FKey EKeys::Ampersand("Ampersand");
const FKey EKeys::Caret("Caret");
const FKey EKeys::Dollar("Dollar");
const FKey EKeys::Exclamation("Exclamation");
const FKey EKeys::Colon("Colon");

const FKey EKeys::A_AccentGrave("A_AccentGrave");
const FKey EKeys::E_AccentGrave("E_AccentGrave");
const FKey EKeys::E_AccentAigu("E_AccentAigu");
const FKey EKeys::C_Cedille("C_Cedille");
const FKey EKeys::Section("Section");

const FKey EKeys::Invalid(NAME_None);

const std::string EKeys::NAME_MouseCategory("Mouse");
const std::string EKeys::NAME_KeyboardCategory("Key");

bool EKeys::bInitialized = false;
std::vector<FKey> EKeys::PrettyOrderedKeys;
std::map<FKey, std::shared_ptr<FKeyDetails> > EKeys::InputKeys;
std::map<std::string, EKeys::FCategoryDisplayInfo> EKeys::MenuCategoryDisplayInfo;


void EKeys::Initialize()
{
	if (bInitialized) return;
	bInitialized = true;

	AddMenuCategoryDisplayInfo(NAME_MouseCategory, "Mouse", "GraphEditor.MouseEvent_16x");
	AddMenuCategoryDisplayInfo(NAME_KeyboardCategory, "Keyboard", "GraphEditor.KeyEvent_16x");
    
	AddKey(FKeyDetails(EKeys::AnyKey, "Any Key",FKeyDetails::KeyboardKey | FKeyDetails::MouseKey |  FKeyDetails::MouseButton));

	AddKey(FKeyDetails(EKeys::MouseX, "Mouse X", FKeyDetails::Axis1D | FKeyDetails::MouseKey));
	AddKey(FKeyDetails(EKeys::MouseY, "Mouse Y", FKeyDetails::Axis1D | FKeyDetails::MouseKey));
	
	AddKey(FKeyDetails(EKeys::MouseScrollUp, "Mouse Wheel Up", FKeyDetails::MouseKey | FKeyDetails::ButtonAxis));
	AddKey(FKeyDetails(EKeys::MouseScrollDown, "Mouse Wheel Down", FKeyDetails::MouseKey | FKeyDetails::ButtonAxis));
	AddKey(FKeyDetails(EKeys::MouseWheelAxis, "Mouse Wheel Axis", FKeyDetails::Axis1D | FKeyDetails::MouseKey));

	AddKey(FKeyDetails(EKeys::LeftMouseButton, "Left Mouse Button", FKeyDetails::MouseKey| FKeyDetails::MouseButton, NAME_None, "LMB"));
	AddKey(FKeyDetails(EKeys::RightMouseButton, "Right Mouse Button", FKeyDetails::MouseKey | FKeyDetails::MouseButton, NAME_None,"RMB"));
	AddKey(FKeyDetails(EKeys::MiddleMouseButton, "Middle Mouse Button", FKeyDetails::MouseKey | FKeyDetails::MouseButton));
	AddKey(FKeyDetails(EKeys::ThumbMouseButton, "Thumb Mouse Button", FKeyDetails::MouseKey | FKeyDetails::MouseButton));
	AddKey(FKeyDetails(EKeys::ThumbMouseButton2, "Thumb Mouse Button 2", FKeyDetails::MouseKey | FKeyDetails::MouseButton));

	AddKey(FKeyDetails(EKeys::Tab, "Tab", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::Enter, "Enter", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::Pause, "Pause", FKeyDetails::KeyboardKey));

	AddKey(FKeyDetails(EKeys::CapsLock,"Caps Lock", FKeyDetails::KeyboardKey, "Caps"));
	AddKey(FKeyDetails(EKeys::Escape, "Escape", FKeyDetails::KeyboardKey, "Esc"));
	AddKey(FKeyDetails(EKeys::SpaceBar,"Space Bar", FKeyDetails::KeyboardKey, "Space"));
	AddKey(FKeyDetails(EKeys::PageUp, "Page Up", FKeyDetails::KeyboardKey, "PgUp"));
	AddKey(FKeyDetails(EKeys::PageDown, "Page Down", FKeyDetails::KeyboardKey, "PgDn"));
	AddKey(FKeyDetails(EKeys::End,"End", FKeyDetails::KeyboardKey, "End"));
	AddKey(FKeyDetails(EKeys::Home, "Home", FKeyDetails::KeyboardKey, "Home"));

	AddKey(FKeyDetails(EKeys::Left, "Left", FKeyDetails::KeyboardKey, "Left"));
	AddKey(FKeyDetails(EKeys::Up, "Up", FKeyDetails::KeyboardKey, "Up"));
	AddKey(FKeyDetails(EKeys::Right, "Right", FKeyDetails::KeyboardKey, "Right"));
	AddKey(FKeyDetails(EKeys::Down, "Down", FKeyDetails::KeyboardKey, "Down"));

	AddKey(FKeyDetails(EKeys::Insert, "Insert", FKeyDetails::KeyboardKey, "Ins"));

    AddKey(FKeyDetails(EKeys::Zero, "0", FKeyDetails::KeyboardKey ));
	AddKey(FKeyDetails(EKeys::One, "1", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::Two, "2", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::Three, "3", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::Four, "4", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::Five, "5", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::Six, "6", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::Seven, "7", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::Eight, "8", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::Nine, "9", FKeyDetails::KeyboardKey));

	AddKey(FKeyDetails(EKeys::A, "A", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::B, "B", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::C, "C", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::D, "D", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::E, "E", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::F, "F", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::G, "G", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::H, "H", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::I, "I", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::J, "J", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::K, "K", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::L, "L", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::M, "M", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::N, "N", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::O, "O", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::P, "P", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::Q, "Q", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::R, "R", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::S, "S", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::T, "T", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::U, "U", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::V, "V", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::W, "W", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::X, "X", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::Y, "Y", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::Z, "Z", FKeyDetails::KeyboardKey));

	AddKey(FKeyDetails(EKeys::NumPadZero, "Num 0", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::NumPadOne, "Num 1", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::NumPadTwo, "Num 2", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::NumPadThree, "Num 3", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::NumPadFour, "Num 4", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::NumPadFive, "Num 5", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::NumPadSix, "Num 6", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::NumPadSeven, "Num 7", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::NumPadEight, "Num 8", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::NumPadNine,"Num 9", FKeyDetails::KeyboardKey));

	AddKey(FKeyDetails(EKeys::Multiply, "Num *", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::Add, "Num +", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::Subtract, "Num -", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::Decimal, "Num .", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::Divide, "Num /", FKeyDetails::KeyboardKey));

	AddKey(FKeyDetails(EKeys::F1, "F1", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::F2, "F2", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::F3, "F3", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::F4, "F4", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::F5, "F5", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::F6, "F6", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::F7, "F7", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::F8, "F8", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::F9, "F9", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::F10, "F10", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::F11, "F11", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::F12, "F12", FKeyDetails::KeyboardKey));

	AddKey(FKeyDetails(EKeys::NumLock, "Num Lock", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::ScrollLock, "Scroll Lock", FKeyDetails::KeyboardKey));

	AddKey(FKeyDetails(EKeys::LeftShift, "Left Shift", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::RightShift, "Right Shift", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::LeftControl, "Left Ctrl", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::RightControl, "Right Ctrl", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::LeftAlt, "Left Alt", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::RightAlt, "Right Alt", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::LeftCommand, "Left Cmd", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::RightCommand, "Right Cmd", FKeyDetails::KeyboardKey));

	AddKey(FKeyDetails(EKeys::Semicolon, "Semicolon", ";", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::Equals, "Equals", "=", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::Comma, "Comma", ",", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::Hyphen, "Hyphen", "-", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::Underscore, "Underscore", "_", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::Period, "Period", ".", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::Slash, "Slash", "/", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::Tilde, "`", FKeyDetails::KeyboardKey)); // Yes this is not actually a tilde, it is a long, sad, and old story
	AddKey(FKeyDetails(EKeys::LeftBracket, "Left Bracket", "[", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::Backslash, "Backslash", "\\", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::RightBracket, "Right Bracket", "]", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::Apostrophe, "Apostrophe", "'", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::Quote, "Quote", "\"", FKeyDetails::KeyboardKey));

	AddKey(FKeyDetails(EKeys::LeftParantheses, "Left Parantheses", "(", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::RightParantheses, "Right Parantheses", ", FKeyDetails::KeyboardKey)", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::Ampersand, "Ampersand", "&", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::Asterix, "Asterisk", "*", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::Caret, "Caret", "^", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::Dollar, "Dollar", "$", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::Exclamation, "Exclamation", "!", FKeyDetails::KeyboardKey));
	AddKey(FKeyDetails(EKeys::Colon, "Colon", ":", FKeyDetails::KeyboardKey));

	AddKey(FKeyDetails(EKeys::A_AccentGrave, std::string(1, char(224))));
	AddKey(FKeyDetails(EKeys::E_AccentGrave, std::string(1, char(232))));
	AddKey(FKeyDetails(EKeys::E_AccentAigu, std::string(1, char(233))));
	AddKey(FKeyDetails(EKeys::C_Cedille, std::string(1, char(231))));
	AddKey(FKeyDetails(EKeys::Section, std::string(1, char(167))));

    // Initialize the input key manager.  This will cause any additional OEM keys to get added
	InputKeyManager::Get();
}


void EKeys::AddKey(const FKeyDetails& KeyDetails)
{
	const FKey& Key = KeyDetails.GetKey();
	//ensureMsgf(!InputKeys.Contains(Key), TEXT("Adding duplicate key '%s'", *Key.ToString());
	Key.KeyDetails.reset(new FKeyDetails(KeyDetails));
	InputKeys[Key] = Key.KeyDetails;

	//add unique key
	if (std::ranges::find(PrettyOrderedKeys, Key) == PrettyOrderedKeys.end()) {
		PrettyOrderedKeys.push_back(Key);
	}
}

void EKeys::GetAllKeys(std::vector<FKey>& OutKeys, uint32 FilterFlag)
{
	if(FilterFlag == FKeyDetails::NoFlags)
	{
		OutKeys = PrettyOrderedKeys;
		return;
	}
	OutKeys.clear();
	std::ranges::copy_if(PrettyOrderedKeys, std::back_inserter(OutKeys), [FilterFlag](FKey & Key)
	{
		const auto FoundedKey = InputKeys.find(Key);
		return FoundedKey!= InputKeys.end() && FoundedKey->second->HasFlags(FilterFlag);
	});
}

void EKeys::RemoveKeysWithCategory(const std::string& InCategory)
{
    std::erase_if(InputKeys, [&InCategory](std::pair<const FKey, std::shared_ptr<FKeyDetails>>& elem) {
            return elem.first.GetMenuCategory() == InCategory; 
        });
}

std::shared_ptr<FKeyDetails> EKeys::GetKeyDetails(const FKey Key)
{
	const auto find_result = InputKeys.find(Key);
	if (find_result == InputKeys.end())
	{
		return {};
	}
	return find_result->second;
}

void EKeys::AddMenuCategoryDisplayInfo(const std::string& CategoryName, const std::string& DisplayName, const std::string& PaletteIcon)
{
	if (MenuCategoryDisplayInfo.contains(CategoryName))
	{
		//UE_LOG(LogInput, Warning, TEXT("Category %s already has menu display info that is being replaced.", *CategoryName.ToString());
	}
	FCategoryDisplayInfo DisplayInfo;
	DisplayInfo.DisplayName = DisplayName;
	DisplayInfo.PaletteIcon = PaletteIcon;
	MenuCategoryDisplayInfo.insert_or_assign(CategoryName,DisplayInfo);
}

std::string EKeys::GetMenuCategoryDisplayName(const std::string& CategoryName)
{
    const auto find_result = MenuCategoryDisplayInfo.find(CategoryName);
	if (find_result != MenuCategoryDisplayInfo.end())
	{
		return find_result->second.DisplayName;
	}
    return MenuCategoryDisplayInfo[NAME_KeyboardCategory].DisplayName;
}