#pragma once
#include <string>
#include "../KeyStruct.h"
#include "../Keys.h"
#include "SimpleMath.h"
#include <cmath>
#include <limits>

typedef unsigned char uint8;
typedef DirectX::SimpleMath::Vector3 FVector;
typedef DirectX::SimpleMath::Vector2 FVector2D;

enum EInputEvent
{
	IE_Pressed              =0,
	IE_Released             =1,
	IE_Repeat               =2,
	IE_DoubleClick          =3,
	IE_Axis                 =4,
	IE_MAX                  =5,
};

struct FInputActionKeyMapping
{
	/** Friendly name of action, e.g "jump" */
	std::string ActionName;


	/** Key to bind it to. */
	FKey Key;

	bool operator==(const FInputActionKeyMapping& Other) const
	{
		return ActionName == Other.ActionName && Key == Other.Key;
	}

	FInputActionKeyMapping(const std::string InActionName = "", const FKey InKey = EKeys::Invalid)
		: ActionName(InActionName)
		, Key(InKey)
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

	FInputAxisKeyMapping(const std::string InAxisName = "", const FKey InKey = EKeys::Invalid, const float InScale = 1.f)
		: AxisName(InAxisName)
		, Scale(InScale)
		, Key(InKey)
	{}
};


/** Paramaters to be considered when calling UPlayerInput::InputKey. */
struct FInputKeyParams
{
	FInputKeyParams() = default;
	
	FInputKeyParams(FKey InKey, enum EInputEvent InEvent, FVector InDelta)
		: Key(InKey)
		, Event(InEvent)
		, Delta(InDelta)
	{};

	FInputKeyParams(FKey InKey, enum EInputEvent InEvent, double InDelta)
		: Key(InKey)
		, Event(InEvent)
		, Delta(FVector(InDelta, 0.0, 0.0))
	{};
	
	FInputKeyParams(FKey InKey, double InDelta, float InDeltaTime, int InNumSamples)
		: Key(InKey)
		, NumSamples(InNumSamples)
		, DeltaTime(InDeltaTime)
		, Delta(FVector(InDelta, 0.0, 0.0))
	{};

	/** The key that has been pressed */
	FKey Key = EKeys::Invalid;

	/** The event that has caused a Button key to be considered */
	enum EInputEvent Event = EInputEvent::IE_Pressed;

	/** The number of samples to be taken into account with the FKeyState::SampleCountAccumulator */
	int NumSamples = 0;

	/** The time between the previous frame and the current one */
	float DeltaTime = 1 / 60.f;
	
	/** The Delta that the given key/axis has been changed by */
	FVector Delta = FVector::Zero;

	/** Get the delta of the given axis for 1D axis */
	double Get1DAxisDelta() const { return Delta.x; }

	/** Get the delta of the given axis for 2D axis */
	FVector2D Get2DAxisDelta() const { return FVector2D((float)Delta.x, (float)Delta.y); }

	/** Get the delta of the given axis for 3D axis */
	FVector Get3DAxisDelta() const { return Delta; }
};