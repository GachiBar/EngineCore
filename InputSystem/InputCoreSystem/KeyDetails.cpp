#include "KeyDetails.h"
#include "Keys.h"

FKeyDetails::FKeyDetails(const FKey InKey, const optional<string>& InLongDisplayName, const uint32 InKeyFlags, const string InMenuCategory, const optional<string>& InShortDisplayName)
	: Key(InKey)
	, MenuCategory(InMenuCategory)
	, LongDisplayName(InLongDisplayName)
	, ShortDisplayName(InShortDisplayName)
{
	CommonInit(InKeyFlags);
}

FKeyDetails::FKeyDetails(const FKey InKey, const optional<string>& InLongDisplayName, const optional<string>& InShortDisplayName, const uint32 InKeyFlags, const std::string InMenuCategory)
	: Key(InKey)
	, MenuCategory(InMenuCategory)
	, LongDisplayName(InLongDisplayName)
	, ShortDisplayName(InShortDisplayName)
{
	CommonInit(InKeyFlags);
}

std::string FKeyDetails::GetDisplayName(const bool bLongDisplayName) const
{
	return bLongDisplayName && LongDisplayName.has_value() ? LongDisplayName.value() : ShortDisplayName.value_or("");
}

void FKeyDetails::CommonInit(const uint32 InKeyFlags)
{
	KeyFlags = InKeyFlags;

	if ((InKeyFlags & EKeyFlags::ButtonAxis) != 0)
	{
		//ensure((InKeyFlags & (EKeyFlags::Axis1D | EKeyFlags::Axis2D | EKeyFlags::Axis3D)) == 0);
		AxisType = EInputAxisType::Button;
	}
	else if ((InKeyFlags & EKeyFlags::Axis1D) != 0)
	{
		//ensure((InKeyFlags & (EKeyFlags::Axis2D | EKeyFlags::Axis3D)) == 0);
		AxisType = EInputAxisType::Axis1D;
	}
	else
	{
		AxisType = EInputAxisType::None;
	}

	// Set up default menu categories
	if (MenuCategory.empty())
	{
        if (IsMouseButton())
		{
			MenuCategory = EKeys::NAME_MouseCategory;
		}
		else
		{
			MenuCategory = EKeys::NAME_KeyboardCategory;
		}
	}
}
