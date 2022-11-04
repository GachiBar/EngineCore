#include "KeyStruct.h"
#include "KeyDetails.h"
#include "Keys.h"
#include <ostream>

const std::string FKey::SyntheticCharPrefix = "UnknownCharCode_";

bool FKey::IsValid() const
{
	if (!KeyName.empty())
	{
		ConditionalLookupKeyDetails();
		return KeyDetails!=nullptr;
	}
	return false;
}

std::string FKey::ToString() const
{
	return KeyName;
}

std::string FKey::GetFName() const
{
	return KeyName;
}

bool FKey::IsMouseButton() const
{
	ConditionalLookupKeyDetails();
	return (KeyDetails ? KeyDetails->IsMouseButton() : false);
}

bool FKey::IsButtonAxis() const
{
	ConditionalLookupKeyDetails();
	return (KeyDetails ? KeyDetails->IsButtonAxis() : false);
}

bool FKey::IsAxis1D() const
{
	ConditionalLookupKeyDetails();
	return (KeyDetails ? KeyDetails->IsAxis1D() : false);
}

bool FKey::IsDigital() const
{
	ConditionalLookupKeyDetails();
	return (KeyDetails ? KeyDetails->IsDigital() : false);
}

bool FKey::IsAnalog() const
{
	ConditionalLookupKeyDetails();
	return (KeyDetails ? KeyDetails->IsAnalog() : false);
}

std::string FKey::GetDisplayName(const bool bLongDisplayName) const
{
	ConditionalLookupKeyDetails();
	return (KeyDetails ? KeyDetails->GetDisplayName(bLongDisplayName) : "");
}

std::string FKey::GetMenuCategory() const
{
	ConditionalLookupKeyDetails();
	return (KeyDetails ? KeyDetails->GetMenuCategory() : EKeys::NAME_KeyboardCategory);
}

void FKey::ConditionalLookupKeyDetails() const
{
	if (!KeyDetails)
	{
		KeyDetails = EKeys::GetKeyDetails(*this);
	}
}

void FKey::ResetKey() const
{
	KeyDetails.reset();
    /*
	const std::string KeyNameStr = KeyName;
	if (KeyName.StartsWith(FKey::SyntheticCharPrefix))
	{
		// This was a dynamically added key, so we need to force it to be synthesized if it doesn't already exist
		const std::string KeyNameStr2 = KeyName.RightChop(FCString::Strlen(FKey::SyntheticCharPrefix));
		const uint32 CharCode = FCString::Atoi(*KeyNameStr2);
		if (CharCode > 0)
		{
			FInputKeyManager::Get().GetKeyFromCodes(INDEX_NONE, CharCode);
		}
	}
    */
}


std::ostream& operator<<(std::ostream& os, const FKey& Key)
{
	if(Key.IsValid())
		os<<Key.ToString();
	return os;
}
