#pragma once
#include <string>
#include "../KeyStruct.h"
#include "../Keys.h"
#include "SimpleMath.h"
#include <cmath>
#include <limits>
#include <utility>

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

/** Paramaters to be considered when calling UPlayerInput::InputKey. */
struct FInputKeyParams
{
	FInputKeyParams() = default;
	
	FInputKeyParams(FKey InKey, enum EInputEvent InEvent, FVector InDelta)
		: Key(std::move(InKey))
		, Event(InEvent)
		, Delta(InDelta)
	{};

	FInputKeyParams(FKey InKey, enum EInputEvent InEvent, double InDelta)
		: Key(InKey)
		, Event(InEvent)
		, Delta(FVector(InDelta, 0.0, 0.0))
	{};

	/** The key that has been pressed */
	FKey Key = EKeys::Invalid;

	/** The event that has caused a Button key to be considered */
	enum EInputEvent Event = EInputEvent::IE_Pressed;

	/** Get the delta of the given axis for 1D axis */
	double Get1DAxisDelta() const { return Delta.x; }

	/** Get the delta of the given axis for 2D axis */
	FVector2D Get2DAxisDelta() const { return FVector2D((float)Delta.x, (float)Delta.y); }


protected:
	/** The Delta that the given key/axis has been changed by */
	FVector Delta = FVector::Zero;
};