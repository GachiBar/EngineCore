
#pragma once
#include <string>
#include <memory>


#include "../../Core/Definitions.h"

struct FKey
{
	FKey()
	= default;

	FKey(std::string InName)
		: KeyName(std::move(InName))
	{
	}

	bool IsValid() const;
	bool IsMouseButton() const;
	bool IsButtonAxis() const;
	bool IsAxis1D() const;
	bool IsDigital() const;
	bool IsAnalog() const;

	std::string GetDisplayName(bool bLongDisplayName = true) const;
	std::string ToString() const;
	std::string GetFName() const;
	std::string GetMenuCategory() const;

	friend bool operator==(const FKey& KeyA, const FKey& KeyB) { return KeyA.KeyName == KeyB.KeyName; }
	friend bool operator!=(const FKey& KeyA, const FKey& KeyB) { return KeyA.KeyName != KeyB.KeyName; }
	friend bool operator<(const FKey& KeyA, const FKey& KeyB) { return KeyA.KeyName < KeyB.KeyName; }
	friend struct EKeys;

    static const std::string SyntheticCharPrefix;

	friend std::ostream& operator<<(std::ostream& os, const FKey& Key);

private:
	std::string KeyName;

	mutable std::shared_ptr<struct FKeyDetails> KeyDetails;

	void ConditionalLookupKeyDetails() const;
	void ResetKey() const;
};