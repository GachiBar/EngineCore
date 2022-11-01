#pragma once
#include <string>
#include "../Keys.h"
#include "../KeyStruct.h"

static const std::string NAME_None;

struct FInputActionKeyMapping
{
	std::string ActionName;

	FKey Key;

	bool operator==(const FInputActionKeyMapping& Other) const
	{
		return (   ActionName == Other.ActionName
				&& Key == Other.Key);
	}

	FInputActionKeyMapping(std::string InActionName = NAME_None, FKey InKey = EKeys::Invalid)
		: ActionName(std::move(InActionName))
		, Key(std::move(InKey))
	{}
};

struct FInputAxisKeyMapping
{
	/** Multiplier to use for the mapping when accumulating the axis value */
	float Scale;

	/** Key to bind it to. */
	FKey Key;

	bool operator==(const FInputAxisKeyMapping& Other) const
	{
		return (Key == Other.Key);
	}

	bool operator<(const FInputAxisKeyMapping& Other) const
	{
		return Key < Other.Key;
	}

	FInputAxisKeyMapping(): Scale(0)
	{
	}

	FInputAxisKeyMapping(FKey InKey = EKeys::Invalid, const float InScale = 1.f)
		: Scale(InScale)
		, Key(std::move(InKey))
	{}

	friend std::ostream& operator<<(std::ostream& os, const FInputAxisKeyMapping& Key);
};

inline std::ostream& operator<<(std::ostream& os, const FInputAxisKeyMapping& Key)
{
	return os;
}
