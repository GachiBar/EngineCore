#include "Keys.h"
#include "KeyStruct.h"
#include "KeyDetails.h"
#include <map>
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
std::map<FKey, std::shared_ptr<FKeyDetails> > EKeys::InputKeys;
std::map<std::string, EKeys::FCategoryDisplayInfo> EKeys::MenuCategoryDisplayInfo;


void EKeys::Initialize()
{
	if (bInitialized) return;
	bInitialized = true;

	AddMenuCategoryDisplayInfo(NAME_MouseCategory, "Mouse", "GraphEditor.MouseEvent_16x");
	AddMenuCategoryDisplayInfo(NAME_KeyboardCategory, "Keyboard", "GraphEditor.KeyEvent_16x");
    
	AddKey(FKeyDetails(EKeys::AnyKey, "Any Key"));

	AddKey(FKeyDetails(EKeys::MouseX, "Mouse X", FKeyDetails::Axis1D | FKeyDetails::MouseButton | FKeyDetails::UpdateAxisWithoutSamples));
	AddKey(FKeyDetails(EKeys::MouseY, "Mouse Y", FKeyDetails::Axis1D | FKeyDetails::MouseButton | FKeyDetails::UpdateAxisWithoutSamples));
	
	AddKey(FKeyDetails(EKeys::MouseScrollUp, "Mouse Wheel Up", FKeyDetails::MouseButton | FKeyDetails::ButtonAxis));
	AddKey(FKeyDetails(EKeys::MouseScrollDown, "Mouse Wheel Down", FKeyDetails::MouseButton | FKeyDetails::ButtonAxis));
	AddKey(FKeyDetails(EKeys::MouseWheelAxis, "Mouse Wheel Axis", FKeyDetails::Axis1D | FKeyDetails::MouseButton | FKeyDetails::UpdateAxisWithoutSamples));

	AddKey(FKeyDetails(EKeys::LeftMouseButton, "Left Mouse Button", FKeyDetails::MouseButton, NAME_None, "LMB"));
	AddKey(FKeyDetails(EKeys::RightMouseButton, "Right Mouse Button", FKeyDetails::MouseButton, NAME_None,"RMB"));
	AddKey(FKeyDetails(EKeys::MiddleMouseButton, "Middle Mouse Button", FKeyDetails::MouseButton));
	AddKey(FKeyDetails(EKeys::ThumbMouseButton, "Thumb Mouse Button", FKeyDetails::MouseButton));
	AddKey(FKeyDetails(EKeys::ThumbMouseButton2, "Thumb Mouse Button 2", FKeyDetails::MouseButton));

	AddKey(FKeyDetails(EKeys::Tab, "Tab"));
	AddKey(FKeyDetails(EKeys::Enter, "Enter"));
	AddKey(FKeyDetails(EKeys::Pause, "Pause"));

	AddKey(FKeyDetails(EKeys::CapsLock,"Caps Lock", "Caps"));
	AddKey(FKeyDetails(EKeys::Escape, "Escape", "Esc"));
	AddKey(FKeyDetails(EKeys::SpaceBar,"Space Bar",  "Space"));
	AddKey(FKeyDetails(EKeys::PageUp, "Page Up", "PgUp"));
	AddKey(FKeyDetails(EKeys::PageDown, "Page Down", "PgDn"));
	AddKey(FKeyDetails(EKeys::End,"End", "End"));
	AddKey(FKeyDetails(EKeys::Home, "Home", "Home"));

	AddKey(FKeyDetails(EKeys::Left, "Left", "Left"));
	AddKey(FKeyDetails(EKeys::Up, "Up", "Up"));
	AddKey(FKeyDetails(EKeys::Right, "Right", "Right"));
	AddKey(FKeyDetails(EKeys::Down, "Down", "Down"));

	AddKey(FKeyDetails(EKeys::Insert, "Insert", "Ins"));

    AddKey(FKeyDetails(EKeys::Zero, "0"));
	AddKey(FKeyDetails(EKeys::One, "1"));
	AddKey(FKeyDetails(EKeys::Two, "2"));
	AddKey(FKeyDetails(EKeys::Three, "3"));
	AddKey(FKeyDetails(EKeys::Four, "4"));
	AddKey(FKeyDetails(EKeys::Five, "5"));
	AddKey(FKeyDetails(EKeys::Six, "6"));
	AddKey(FKeyDetails(EKeys::Seven, "7"));
	AddKey(FKeyDetails(EKeys::Eight, "8"));
	AddKey(FKeyDetails(EKeys::Nine, "9"));

	AddKey(FKeyDetails(EKeys::A, "A"));
	AddKey(FKeyDetails(EKeys::B, "B"));
	AddKey(FKeyDetails(EKeys::C, "C"));
	AddKey(FKeyDetails(EKeys::D, "D"));
	AddKey(FKeyDetails(EKeys::E, "E"));
	AddKey(FKeyDetails(EKeys::F, "F"));
	AddKey(FKeyDetails(EKeys::G, "G"));
	AddKey(FKeyDetails(EKeys::H, "H"));
	AddKey(FKeyDetails(EKeys::I, "I"));
	AddKey(FKeyDetails(EKeys::J, "J"));
	AddKey(FKeyDetails(EKeys::K, "K"));
	AddKey(FKeyDetails(EKeys::L, "L"));
	AddKey(FKeyDetails(EKeys::M, "M"));
	AddKey(FKeyDetails(EKeys::N, "N"));
	AddKey(FKeyDetails(EKeys::O, "O"));
	AddKey(FKeyDetails(EKeys::P, "P"));
	AddKey(FKeyDetails(EKeys::Q, "Q"));
	AddKey(FKeyDetails(EKeys::R, "R"));
	AddKey(FKeyDetails(EKeys::S, "S"));
	AddKey(FKeyDetails(EKeys::T, "T"));
	AddKey(FKeyDetails(EKeys::U, "U"));
	AddKey(FKeyDetails(EKeys::V, "V"));
	AddKey(FKeyDetails(EKeys::W, "W"));
	AddKey(FKeyDetails(EKeys::X, "X"));
	AddKey(FKeyDetails(EKeys::Y, "Y"));
	AddKey(FKeyDetails(EKeys::Z, "Z"));

	AddKey(FKeyDetails(EKeys::NumPadZero, "Num 0"));
	AddKey(FKeyDetails(EKeys::NumPadOne, "Num 1"));
	AddKey(FKeyDetails(EKeys::NumPadTwo, "Num 2"));
	AddKey(FKeyDetails(EKeys::NumPadThree, "Num 3"));
	AddKey(FKeyDetails(EKeys::NumPadFour, "Num 4"));
	AddKey(FKeyDetails(EKeys::NumPadFive, "Num 5"));
	AddKey(FKeyDetails(EKeys::NumPadSix, "Num 6"));
	AddKey(FKeyDetails(EKeys::NumPadSeven, "Num 7"));
	AddKey(FKeyDetails(EKeys::NumPadEight, "Num 8"));
	AddKey(FKeyDetails(EKeys::NumPadNine,"Num 9"));

	AddKey(FKeyDetails(EKeys::Multiply, "Num *"));
	AddKey(FKeyDetails(EKeys::Add, "Num +"));
	AddKey(FKeyDetails(EKeys::Subtract, "Num -"));
	AddKey(FKeyDetails(EKeys::Decimal, "Num ."));
	AddKey(FKeyDetails(EKeys::Divide, "Num /"));

	AddKey(FKeyDetails(EKeys::F1, "F1"));
	AddKey(FKeyDetails(EKeys::F2, "F2"));
	AddKey(FKeyDetails(EKeys::F3, "F3"));
	AddKey(FKeyDetails(EKeys::F4, "F4"));
	AddKey(FKeyDetails(EKeys::F5, "F5"));
	AddKey(FKeyDetails(EKeys::F6, "F6"));
	AddKey(FKeyDetails(EKeys::F7, "F7"));
	AddKey(FKeyDetails(EKeys::F8, "F8"));
	AddKey(FKeyDetails(EKeys::F9, "F9"));
	AddKey(FKeyDetails(EKeys::F10, "F10"));
	AddKey(FKeyDetails(EKeys::F11, "F11"));
	AddKey(FKeyDetails(EKeys::F12, "F12"));

	AddKey(FKeyDetails(EKeys::NumLock, "Num Lock"));
	AddKey(FKeyDetails(EKeys::ScrollLock, "Scroll Lock"));

	AddKey(FKeyDetails(EKeys::LeftShift, "Left Shift", FKeyDetails::ModifierKey));
	AddKey(FKeyDetails(EKeys::RightShift, "Right Shift", FKeyDetails::ModifierKey));
	AddKey(FKeyDetails(EKeys::LeftControl, "Left Ctrl", FKeyDetails::ModifierKey));
	AddKey(FKeyDetails(EKeys::RightControl, "Right Ctrl", FKeyDetails::ModifierKey));
	AddKey(FKeyDetails(EKeys::LeftAlt, "Left Alt", FKeyDetails::ModifierKey));
	AddKey(FKeyDetails(EKeys::RightAlt, "Right Alt", FKeyDetails::ModifierKey));
	AddKey(FKeyDetails(EKeys::LeftCommand, "Left Cmd", FKeyDetails::ModifierKey));
	AddKey(FKeyDetails(EKeys::RightCommand, "Right Cmd", FKeyDetails::ModifierKey));

	AddKey(FKeyDetails(EKeys::Semicolon, "Semicolon", ";"));
	AddKey(FKeyDetails(EKeys::Equals, "Equals", "="));
	AddKey(FKeyDetails(EKeys::Comma, "Comma", ","));
	AddKey(FKeyDetails(EKeys::Hyphen, "Hyphen", "-"));
	AddKey(FKeyDetails(EKeys::Underscore, "Underscore", "_"));
	AddKey(FKeyDetails(EKeys::Period, "Period", "."));
	AddKey(FKeyDetails(EKeys::Slash, "Slash", "/"));
	AddKey(FKeyDetails(EKeys::Tilde, "`")); // Yes this is not actually a tilde, it is a long, sad, and old story
	AddKey(FKeyDetails(EKeys::LeftBracket, "Left Bracket", "["));
	AddKey(FKeyDetails(EKeys::Backslash, "Backslash", "\\"));
	AddKey(FKeyDetails(EKeys::RightBracket, "Right Bracket", "]"));
	AddKey(FKeyDetails(EKeys::Apostrophe, "Apostrophe", "'"));
	AddKey(FKeyDetails(EKeys::Quote, "Quote", "\""));

	AddKey(FKeyDetails(EKeys::LeftParantheses, "Left Parantheses", "("));
	AddKey(FKeyDetails(EKeys::RightParantheses, "Right Parantheses", ")"));
	AddKey(FKeyDetails(EKeys::Ampersand, "Ampersand", "&"));
	AddKey(FKeyDetails(EKeys::Asterix, "Asterisk", "*"));
	AddKey(FKeyDetails(EKeys::Caret, "Caret", "^"));
	AddKey(FKeyDetails(EKeys::Dollar, "Dollar", "$"));
	AddKey(FKeyDetails(EKeys::Exclamation, "Exclamation", "!"));
	AddKey(FKeyDetails(EKeys::Colon, "Colon", ":"));

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
}

void EKeys::GetAllKeys(std::vector<FKey>& OutKeys)
{
    OutKeys.clear();

    for (auto& it : InputKeys)
	{
		OutKeys.push_back(it.first);
	}
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