#pragma once
#include <set>
#include "../KeyStruct.h"

typedef unsigned int uint32;

struct FInputEvent
{
	FInputEvent():bIsRepeat(false)
	{ }

    FInputEvent(bool bInIsRepeat):bIsRepeat(bInIsRepeat)
	{ }

	/**
	 * Virtual destructor.
	 */
	virtual ~FInputEvent( ) { }

public:

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
protected:
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
		: FInputEvent(false)
		, Character(0)
	{
	}

	FCharacterEvent(const wchar_t InCharacter, const bool bInIsRepeat)
		: FInputEvent(bInIsRepeat)
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
		: FInputEvent(false)
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
				const uint32 InCharacterCode,
				const uint32 InKeyCode
	)
		: FInputEvent(bInIsRepeat)
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
		, WheelDelta(0.0f, 0.0f)
		, bIsDirectionInvertedFromDevice(false)
	{ }

	/** Events are immutable once constructed. */
	FPointerEvent(
		const FVector2D& InScreenSpacePosition,
		const FVector2D& InLastScreenSpacePosition,
		const std::set<FKey>& InPressedButtons,
		FKey InEffectingButton,
		float InWheelDelta
	)
		: FInputEvent(false)
		, ScreenSpacePosition(InScreenSpacePosition)
		, LastScreenSpacePosition(InLastScreenSpacePosition)
		, CursorDelta(InScreenSpacePosition - InLastScreenSpacePosition)
		, PressedButtons(&InPressedButtons)
		, EffectingButton(InEffectingButton)
		, bIsDirectionInvertedFromDevice(false)
	{ }

	FPointerEvent(
		const FVector2D& InScreenSpacePosition,
		const FVector2D& InLastScreenSpacePosition,
		const FVector2D& InDelta,
		const std::set<FKey>& InPressedButtons
	)
		: FInputEvent(false)
		, ScreenSpacePosition(InScreenSpacePosition)
		, LastScreenSpacePosition(InLastScreenSpacePosition)
		, CursorDelta(InDelta)
		, PressedButtons(&InPressedButtons)
		, bIsDirectionInvertedFromDevice(false)
	{ }

	/** A constructor for raw mouse events */
	FPointerEvent(
		const FVector2D& InScreenSpacePosition,
		const FVector2D& InLastScreenSpacePosition,
		const FVector2D& InDelta,
		const std::set<FKey>& InPressedButtons
	)
		: FInputEvent(false)
		, ScreenSpacePosition(InScreenSpacePosition)
		, LastScreenSpacePosition(InLastScreenSpacePosition)
		, CursorDelta(InDelta)
		, PressedButtons(&InPressedButtons)
		, WheelDelta(0.0f, 0.0f)
		, bIsDirectionInvertedFromDevice(false)
	{ }


	/** A constructor to alter cursor positions */
	FPointerEvent(
		const FPointerEvent& Other,
		const FVector2D& InScreenSpacePosition,
		const FVector2D& InLastScreenSpacePosition)
	{
		*this = Other;
		ScreenSpacePosition = InScreenSpacePosition;
		LastScreenSpacePosition = InLastScreenSpacePosition;
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
	float GetWheelDelta() const { return WheelDelta.Y; }

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

private:

	FVector2D ScreenSpacePosition;
	FVector2D LastScreenSpacePosition;
	FVector2D CursorDelta;
	std::set<FKey> PressedButtons;
	FKey EffectingButton;

	FVector2D WheelDelta;
	bool bIsDirectionInvertedFromDevice;
};
