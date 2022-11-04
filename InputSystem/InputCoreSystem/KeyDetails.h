#pragma once
#include <string>
#include "KeyStruct.h"
#include <optional>
#include "../../Core/Definitions.h"

using std::optional;
using std::string;

#define BIT_FLAG(x) 1<<x

struct FKeyDetails
{
	enum EKeyFlags
	{
		KeyboardKey				= BIT_FLAG(3),
		MouseKey				= BIT_FLAG(4),

		MouseButton				= BIT_FLAG(5),
		ButtonAxis				= BIT_FLAG(6),
		Axis1D					= BIT_FLAG(7),
		NoFlags                  = 0,
	};

	FKeyDetails(const FKey InKey, const optional<string>& InLongDisplayName, const uint32 InKeyFlags = 0, const string InMenuCategory = "", const optional<string>& InShortDisplayName = "" );
	FKeyDetails(const FKey InKey, const optional<string>& InLongDisplayName, const optional<string>& InShortDisplayName, const uint32 InKeyFlags = 0, const std::string InMenuCategory = "");

	bool IsMouseButton() const { return ( KeyFlags & EKeyFlags::MouseButton) != 0; }
	bool IsAxis1D() const { return AxisType == EInputAxisType::Axis1D; }
	bool IsButtonAxis() const { return AxisType == EInputAxisType::Button; }	// Analog 1D axis emulating a digital button press.
	bool IsAnalog() const { return IsAxis1D(); }
	bool IsDigital() const { return !IsAnalog(); }
	std::string GetMenuCategory() const { return MenuCategory; }
	std::string GetDisplayName(const bool bLongDisplayName = true) const;
	const FKey& GetKey() const { return Key; }

	bool HasFlags(const uint32 InKeyFlags) const
	{
		return KeyFlags & InKeyFlags;
	}

private:
	friend struct EKeys;

	void CommonInit(const uint32 InKeyFlags);

	enum class EInputAxisType : uint8
	{
		None,
		Button,			// Whilst the physical input is an analog axis the FKey uses it to emulate a digital button.
		Axis1D
	};

	FKey  Key;

	std::string MenuCategory;

	EInputAxisType AxisType;

	uint32 KeyFlags;

	optional<string> LongDisplayName;
	optional<string> ShortDisplayName;
};

#undef BIT_FLAG