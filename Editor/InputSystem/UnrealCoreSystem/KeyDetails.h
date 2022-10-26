#pragma once
#include <string>
#include "KeyStruct.h"
#include <optional>

typedef unsigned char uint8;

using std::optional;
using std::string;

#define BIT_FLAG(x) 1<<x

struct FKeyDetails
{
	enum EKeyFlags
	{
		MouseButton				 = BIT_FLAG(2),
		ModifierKey				 = BIT_FLAG(3),
		Axis1D					 = BIT_FLAG(5),
		UpdateAxisWithoutSamples = BIT_FLAG(7),
		NotActionBindableKey	 = BIT_FLAG(8),
		Deprecated				 = BIT_FLAG(9),

		// All axis representations
		ButtonAxis				 = BIT_FLAG(10),		// Analog 1D axis emulating a digital button press. E.g. Gamepad right stick up
		Axis2D					 = BIT_FLAG(11),

		NoFlags                  = 0,
	};

	FKeyDetails(const FKey InKey, const optional<string>& InLongDisplayName, const uint32 InKeyFlags = 0, const string InMenuCategory = "", const optional<string>& InShortDisplayName = "" );
	FKeyDetails(const FKey InKey, const optional<string>& InLongDisplayName, const optional<string>& InShortDisplayName, const uint32 InKeyFlags = 0, const std::string InMenuCategory = "");

	bool IsMouseButton() const { return bIsMouseButton != 0; }
	bool IsAxis1D() const { return AxisType == EInputAxisType::Axis1D; }
	bool IsAxis2D() const { return AxisType == EInputAxisType::Axis2D; }
	bool IsButtonAxis() const { return AxisType == EInputAxisType::Button; }	// Analog 1D axis emulating a digital button press.
	bool IsAnalog() const { return IsAxis1D() || IsAxis2D(); }
	bool IsDigital() const { return !IsAnalog(); }
	bool ShouldUpdateAxisWithoutSamples() const { return bShouldUpdateAxisWithoutSamples != 0; }
	bool IsBindableToActions() const { return bIsBindableToActions != 0; }
	std::string GetMenuCategory() const { return MenuCategory; }
	std::string GetDisplayName(const bool bLongDisplayName = true) const;
	const FKey& GetKey() const { return Key; }

	// Key pairing
	EPairedAxis GetPairedAxis() const { return PairedAxis; }
	const FKey& GetPairedAxisKey() const { return PairedAxisKey; }

private:
	friend struct EKeys;

	void CommonInit(const uint32 InKeyFlags);

	enum class EInputAxisType : uint8
	{
		None,
		Button,			// Whilst the physical input is an analog axis the FKey uses it to emulate a digital button.
		Axis1D,
		Axis2D,
	};

	FKey  Key;

	// Key pairing
	EPairedAxis PairedAxis = EPairedAxis::Unpaired;		// Paired axis identifier. Lets this key know which axis it represents on the PairedAxisKey
	FKey		PairedAxisKey;							// Paired axis reference. This is the FKey representing the final paired vector axis. Note: NOT the other key in the pairing.

	std::string MenuCategory;

	uint8 bIsMouseButton : 1;
	uint8 bShouldUpdateAxisWithoutSamples : 1;
	uint8 bIsBindableToActions : 1;
	EInputAxisType AxisType;

	optional<string> LongDisplayName;
	optional<string> ShortDisplayName;
};

#undef BIT_FLAG