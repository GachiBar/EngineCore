
#pragma once
#include <string>
#include <memory>

typedef unsigned char uint8;
typedef unsigned int uint32;

enum class EPairedAxis : uint8
{
	Unpaired,			// This key is unpaired
	X,					// This key represents the X axis of its PairedAxisKey
	Y					// This key represents the Y axis of its PairedAxisKey
};

struct FKey
{
	FKey()
	{
	}

	FKey(std::string InName)
		: KeyName(std::move(InName))
	{
	}

	bool IsValid() const;
	bool IsMouseButton() const;
	bool IsButtonAxis() const;
	bool IsAxis1D() const;
	bool IsAxis2D() const;
	bool IsDigital() const;
	bool IsAnalog() const;
	bool ShouldUpdateAxisWithoutSamples() const;
	bool IsBindableToActions() const;

	std::string GetDisplayName(bool bLongDisplayName = true) const;
	std::string ToString() const;
	std::string GetFName() const;
	std::string GetMenuCategory() const;
	EPairedAxis GetPairedAxis() const;
	FKey GetPairedAxisKey() const;

	friend bool operator==(const FKey& KeyA, const FKey& KeyB) { return KeyA.KeyName == KeyB.KeyName; }
	friend bool operator!=(const FKey& KeyA, const FKey& KeyB) { return KeyA.KeyName != KeyB.KeyName; }

	friend struct EKeys;

    static const std::string SyntheticCharPrefix;
private:
	std::string KeyName;

	mutable class std::shared_ptr<struct FKeyDetails> KeyDetails;

	void ConditionalLookupKeyDetails() const;
	void ResetKey() const;
};