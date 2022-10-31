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
	/** Friendly name of axis, e.g "MoveForward" */
	std::string AxisName;

	/** Multiplier to use for the mapping when accumulating the axis value */
	float Scale;

	/** Key to bind it to. */
	FKey Key;

	bool operator==(const FInputAxisKeyMapping& Other) const
	{
		//TODO move to math lib
		auto IsNearlyEqual = [](float A, float B, float ErrorTolerance = std::numeric_limits<float>::epsilon())
		{
			return std::abs(A - B) <= ErrorTolerance;
		};

		return (   AxisName == Other.AxisName
				&& Key == Other.Key
				&& IsNearlyEqual (Scale, Other.Scale) );
	}

	bool operator<(const FInputAxisKeyMapping& Other) const
	{
		return AxisName < Other.AxisName
			&& Key < Other.Key
			&& Scale < Other.Scale;
	}

	FInputAxisKeyMapping(): Scale(0)
	{
	}

	FInputAxisKeyMapping(std::string InAxisName = NAME_None, FKey InKey = EKeys::Invalid, const float InScale = 1.f)
		: AxisName(std::move(InAxisName))
		, Scale(InScale)
		, Key(std::move(InKey))
	{}
};
