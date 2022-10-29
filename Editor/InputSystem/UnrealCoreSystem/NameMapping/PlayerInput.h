#pragma once
#include <map>
#include <set>
#include <vector>

#include "NamingStructs.h"

#include "../KeyStruct.h"

/*=============================================================================
	 KeyState - contains details about a key's state and recent events
=============================================================================*/

class InputSettings;

struct FKeyState
{
	/** This is the most recent raw value reported by the device.  For digital buttons, 0 or 1.  For analog buttons, 0->1.  For axes, -1->1. The X field is for non-vector keys */
	FVector RawValue;

	/** The final "value" for this control, after any optional processing. */
	FVector Value;

	/** Global time of last up->down or down->up transition. */
	float LastUpDownTransitionTime;

	/** True if this key is "down", false otherwise. */
	uint8 bDown:1;

	/** Queued state information.  This data is updated or flushed once player input is processed. */
	uint8 bDownPrevious:1;

	/** True if this key has been "consumed" by an InputComponent and should be ignored for further components during this update. */
	uint8 bConsumed:1;

	/** Flag paired axes that have been sampled this tick. X = LSB, Z = MSB */
	uint8 PairSampledAxes : 3;

	/** How many of each event type had been received when input was last processed. */
	std::vector<uint32> EventCounts[IE_MAX];

	/** Used to accumulate events during the frame and flushed when processed. */
	std::vector<uint32> EventAccumulator[IE_MAX];

	/** Used to accumulate input values during the frame and flushed after processing. */
	FVector RawValueAccumulator;

	FKeyState()
		: RawValue(0.f, 0.f, 0.f)
		, Value(0.f, 0.f, 0.f)
		, LastUpDownTransitionTime(0.f)
		, bDown(false)
		, bDownPrevious(false)
		, bConsumed(false)
		, PairSampledAxes(0)
		, RawValueAccumulator(0.f, 0.f, 0.f)
	{
	}
};

class PlayerInput
{
public:

	/** @return true if InKey is currently held */
	bool IsPressed(FKey InKey) const;

	/** @return true if InKey went from up to down since player input was last processed. */
	bool WasJustPressed(FKey InKey) const;

	/** return true if InKey went from down to up since player input was last processed. */
	bool WasJustReleased(FKey InKey) const;

	/** @return current state of the InKey */
	float GetKeyValue(FKey InKey) const;

	/** @return processed value of the InKey */
	FVector GetProcessedVectorKeyValue(FKey InKey) const;

    void Proccess();

	bool InputKey(FKey Key, EInputEvent Event, float AmountDepressed);
	bool InputKey(const FInputKeyParams& Params);



protected:
	/** @return True if a key is handled by an action binding */
	bool IsKeyHandledByAction(FKey Key) const;

	/* Returns the summed values of all the components of this axis this frame
	 * @param AxisBinding - the action to determine if it ocurred
	 * @param KeysToConsume - array to collect the keys associated with this binding that should be consumed
	 */
	float DetermineAxisValue(const std::string& AxisName, std::set<FKey>& KeysToConsume);

	
	/** A counter used to track the order in which events occurred since the last time the input stack was processed */
	uint32 EventCount = 0;

protected:
	std::shared_ptr<InputSettings> InputSettings;
    /** The current game view of each key */
	std::map<FKey,FKeyState> KeyStateMap;
};