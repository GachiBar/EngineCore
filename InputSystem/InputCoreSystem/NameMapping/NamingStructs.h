#pragma once
#include "../KeyStruct.h"
#include "../Keys.h"
#include "SimpleMath.h"
#include <utility>
#include "../../../Core/Definitions.h"

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
	
	FInputKeyParams(FKey InKey, enum EInputEvent InEvent, FVector2D InDelta)
		: Key(std::move(InKey))
		, Event(InEvent)
		, Delta(InDelta)
	{};

	FInputKeyParams(FKey InKey, enum EInputEvent InEvent, float InDelta)
		: Key(std::move(InKey))
		, Event(InEvent)
		, Delta(FVector2D(InDelta, 0.0f))
	{};

	/** The key that has been pressed */
	FKey Key = EKeys::Invalid;

	/** The event that has caused a Button key to be considered */
	enum EInputEvent Event = EInputEvent::IE_Pressed;

	/** Get the delta of the given axis for 1D axis */
	double Get1DAxisDelta() const { return Delta.x; }

	/** Get the delta of the given axis for 2D axis */
	FVector2D Get2DAxisDelta() const { return FVector2D((float)Delta.x, (float)Delta.y); }

	FVector2D Delta = FVector2D::Zero;
protected:
	/** The Delta that the given key/axis has been changed by */
	
};