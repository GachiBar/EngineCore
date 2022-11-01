#pragma once
#include <map>
#include <set>
#include <vector>

#include "NamingStructs.h"

#include "../KeyStruct.h"

/*=============================================================================
	 KeyState - contains details about a key's state and recent events
=============================================================================*/

class FWindowsWindow;
class FGenericApplicationMessageHandler;
class InputSettings;

struct FKeyState
{
	/** The final "value" for this control, after any optional processing. */
	FVector Value;

	/** True if this key is "down", false otherwise. */
	uint8 bDown:1;

	/** Queued state information */
	uint8 bDownPrevious:1;

	int EventCounts[IE_MAX] ;

	void FlushEvents()
	{
		std::fill_n(EventCounts, IE_MAX, 0);
	}

	FKeyState()
		: Value(0.f, 0.f, 0.f)
		, bDown(false)
		, bDownPrevious(false)
	{
		std::fill_n(EventCounts, IE_MAX, 0);
	}
};

class PlayerInput
{
public:

	PlayerInput();

	/** @return true if InKey is currently held */
	bool IsPressed(FKey InKey) const;

	/** @return true if InKey went from up to down since player input was last processed. */
	bool WasJustPressed(FKey InKey) const;

	/** return true if InKey went from down to up since player input was last processed. */
	bool WasJustReleased(FKey InKey) const;

	bool IsActionPressed(const std::string& ActionName) const;

	bool WasActionJustPressed(const std::string& ActionName) const;

	bool WasActionJustReleased(const std::string& ActionName) const;

	/** @return current state of the InKey */
	float GetKeyValue(FKey InKey) const;

	float GetAxisValue(std::string const& AxisName) const;
	/** @return processed value of the InKey */
	FVector GetProcessedVectorKeyValue(FKey InKey) const;

	std::shared_ptr<FGenericApplicationMessageHandler> MessageHandler;

	void SetInputSettings(InputSettings* InInputSettings);

	std::map<FKey, FKeyState>& GetKeyStates();

	void SetKeyStateValue(FKey const& Key, float Value);
protected:
	/** @return True if a key is handled by an action binding */
	bool IsKeyHandledByAction(FKey Key) const;

	/* Returns the summed values of all the components of this axis this frame
	 * @param KeysToConsume - array to collect the keys associated with this binding that should be consumed
	 */
	float DetermineAxisValue(const std::string& AxisName, std::set<FKey>& KeysToConsume);

	
	/** A counter used to track the order in which events occurred since the last time the input stack was processed */
	uint32 EventCount = 0;

protected:
	InputSettings* input_settings{};

    /** The current game view of each key */
	std::map<FKey,FKeyState> KeyStateMap{};
};