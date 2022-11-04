#pragma once
#include <map>
#include <set>
#include "../KeyStruct.h"
#include "../NameMapping/NamingStructs.h"
#include "../NameMapping/PlayerInput.h"
#include "../../../Core/Definitions.h"

struct FKeyState;

struct FInputEvent
{
	FInputEvent():InputEvent(IE_MAX),bIsRepeat(false)
	{ }

    FInputEvent(EInputEvent InInputEvent, bool bInIsRepeat):InputEvent(InInputEvent), bIsRepeat(bInIsRepeat)
	{ }

	/**
	 * Virtual destructor.
	 */
	virtual ~FInputEvent( ) = default;

public:

	virtual void OnApplyInput(std::map<FKey, FKeyState>& KeyStateMap)
	{
		
	}


	/**
	 * Returns whether or not this character is an auto-repeated keystroke
	 *
	 * @return  True if this character is a repeat
	 */
	bool IsRepeat() const
	{
		return bIsRepeat;
	}

    	/** Is this event a pointer event (touch or cursor). */
	virtual bool IsPointerEvent() const
    {
        return false;
    }

	/** Is this event a key event. */
	virtual bool IsKeyEvent() const
    {
        return false;
    }

	float GetFloatValueFromInputEvent(EInputEvent IE) const
	{
		switch (IE) {
			case IE_Released:
				return 0.f;
			case IE_Pressed:
			case IE_Repeat:
			case IE_DoubleClick:
				return 1.f;
			case IE_Axis:
			case IE_MAX:
			default:
				return 0.f;
		}
	}

protected:
	EInputEvent InputEvent;
	// True if this key was auto-repeated.
	bool bIsRepeat;
};

struct FCharacterEvent
	: public FInputEvent
{
	/**
	 * UStruct Constructor.  Not meant for normal usage.
	 */
	FCharacterEvent()
		: FInputEvent()
		, Character(0)
	{
	}

	FCharacterEvent(const wchar_t InCharacter, const bool bInIsRepeat)
		: FInputEvent(bInIsRepeat? IE_Repeat : IE_Pressed, bInIsRepeat)
		, Character(InCharacter)
	{ }

	/**
	 * Returns the character for this event
	 *
	 * @return  Character
	 */
	wchar_t GetCharacter() const
	{
		return Character;
	}

	virtual void OnApplyInput(std::map<FKey, FKeyState>& KeyStateMap) override
	{

	}
private:

	// The character that was pressed.
	wchar_t Character;
};

struct FKeyEvent : public FInputEvent
{
	/**
	 * UStruct Constructor.  Not meant for normal usage.
	 */
	FKeyEvent()
		: FInputEvent()
		, Key()
		, CharacterCode(0)
		, KeyCode(0)
	{
	}

	/**
	 * Constructor.  Events are immutable once constructed.
	 *
	 * @param  InKeyName  Character name
	 * @param  bInIsRepeat  True if this key is an auto-repeated keystroke
	 */
	FKeyEvent(	const FKey InKey,
				const bool bInIsRepeat,
				const EInputEvent InInputEvent,
				const uint32 InCharacterCode,
				const uint32 InKeyCode
	)
		: FInputEvent(InInputEvent,bInIsRepeat)
		, Key(InKey)
		, CharacterCode(InCharacterCode)
		, KeyCode(InKeyCode)
	{ }

	/**
	 * Returns the name of the key for this event
	 *
	 * @return  Key name
	 */
	FKey GetKey() const
	{
		return Key;
	}

	/**
	 * Returns the character code for this event.
	 *
	 * @return  Character code or 0 if this event was not a character key press
	 */
	uint32 GetCharacter() const
	{
		return CharacterCode;
	}

	/**
	 * Returns the key code received from hardware before any conversion/mapping.
	 *
	 * @return  Key code received from hardware
	 */
	uint32 GetKeyCode() const
	{
		return KeyCode;
	}

	virtual bool IsKeyEvent() const override
	{
		return true;
	}

	virtual void OnApplyInput(std::map<FKey, FKeyState>& KeyStateMap) override
	{
		FKeyState& KeyState = KeyStateMap[Key];

		KeyState.bDownPrevious = KeyState.bDown;
		KeyState.bDown = InputEvent == IE_Repeat || InputEvent == IE_Pressed;

		KeyState.EventCounts[InputEvent]++;

		KeyState.Value.y = 0;
		KeyState.Value.x = GetFloatValueFromInputEvent(InputEvent);
	}

private:
	// Name of the key that was pressed.
	FKey Key;

	// The character code of the key that was pressed.  Only applicable to typed character keys, 0 otherwise.
	uint32 CharacterCode;

	// Original key code received from hardware before any conversion/mapping
	uint32 KeyCode;
};

struct FPointerEvent
	: public FInputEvent
{
	FPointerEvent()
		: ScreenSpacePosition(FVector2D(0, 0))
		, LastScreenSpacePosition(FVector2D(0, 0))
		, CursorDelta(FVector2D(0, 0))
		, PressedButtons()
		, EffectingButton()
		, WheelDelta(0.0f)
		, bIsDirectionInvertedFromDevice(false)
	{ }

	/** Events are immutable once constructed. */
	FPointerEvent(
		const EInputEvent InInputEvent,
		const FVector2D& InScreenSpacePosition,
		const FVector2D& InLastScreenSpacePosition,
		const std::set<FKey>& InPressedButtons,
		const FKey InEffectingButton,
		float InWheelDelta
	)
		: FInputEvent(InInputEvent,false)
		, ScreenSpacePosition(InScreenSpacePosition)
		, LastScreenSpacePosition(InLastScreenSpacePosition)
		, CursorDelta(InScreenSpacePosition - InLastScreenSpacePosition)
		, PressedButtons(InPressedButtons)
		, EffectingButton(std::move(InEffectingButton))
		, WheelDelta(InWheelDelta)
		,bIsDirectionInvertedFromDevice(false)
	{ }

	FPointerEvent(
		const FPointerEvent& InPointerEvent,
		EInputEvent InInputEvent,
		const FKey InEffectingButton
	)
	{
		*this = InPointerEvent;
		this->InputEvent = InInputEvent;
		this->EffectingButton = InEffectingButton;
	}
	
public:

	/** Returns The position of the cursor in screen space */
	const FVector2D& GetScreenSpacePosition() const { return ScreenSpacePosition; }

	/** Returns the position of the cursor in screen space last time we handled an input event */
	const FVector2D& GetLastScreenSpacePosition() const { return LastScreenSpacePosition; }

	/** Returns the distance the mouse traveled since the last event was handled. */
	const FVector2D& GetCursorDelta() const { return CursorDelta; }

	/** Mouse buttons that are currently pressed */
	bool IsMouseButtonDown( FKey const & MouseButton ) const { return PressedButtons.contains( MouseButton ); }

	/** Mouse button that caused this event to be raised (possibly FKey::Invalid) */
	FKey GetEffectingButton() const { return EffectingButton; }
	
	/** How much did the mouse wheel turn since the last mouse event */
	float GetWheelDelta() const { return WheelDelta; }

	/** Returns the full set of pressed buttons */
	const std::set<FKey>& GetPressedButtons() const { return PressedButtons; }

	/** We override the assignment operator to allow generated code to compile with the const ref member. */
	void operator=( const FPointerEvent& Other )
	{
		FInputEvent::operator=( Other );

		// Pointer
		ScreenSpacePosition = Other.ScreenSpacePosition;
		LastScreenSpacePosition = Other.LastScreenSpacePosition;
		CursorDelta = Other.CursorDelta;
		PressedButtons = Other.PressedButtons;
		EffectingButton = Other.EffectingButton;
		WheelDelta = Other.WheelDelta;
		bIsDirectionInvertedFromDevice = Other.bIsDirectionInvertedFromDevice;
	}

	virtual bool IsPointerEvent() const override
	{
		return true;
	}

	virtual void OnApplyInput(std::map<FKey, FKeyState>& KeyStateMap) override
	{
		FKeyState& KeyState = KeyStateMap[EffectingButton];

		KeyState.bDownPrevious = KeyState.bDown;
		KeyState.bDown = InputEvent == IE_Repeat || InputEvent == IE_Pressed;

		KeyState.EventCounts[InputEvent]++;

		if(InputEvent==IE_Axis)
		{
			KeyState.Value.y = 0;
			if(EffectingButton==EKeys::MouseX)
			{
				KeyState.Value.x = CursorDelta.x;
			}
			else if (EffectingButton == EKeys::MouseY)
			{
				KeyState.Value.x = CursorDelta.y;
			}
			else if(EffectingButton == EKeys::MouseWheelAxis)
			{
				KeyState.Value.x = WheelDelta;
			}
		}
		else
		{
			KeyState.Value.y = 0;
			KeyState.Value.x = GetFloatValueFromInputEvent(InputEvent);
		}
	}

private:

	FVector2D ScreenSpacePosition;
	FVector2D LastScreenSpacePosition;
	FVector2D CursorDelta;
	std::set<FKey> PressedButtons{};
	FKey EffectingButton;

	float WheelDelta{};
	bool bIsDirectionInvertedFromDevice{};
};
