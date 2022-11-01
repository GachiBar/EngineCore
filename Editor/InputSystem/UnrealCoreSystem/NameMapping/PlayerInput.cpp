#include "PlayerInput.h"

#include <algorithm>
#include "ActionMapping.h"
#include "../MessageHandler/WindowsApplicationMessageHandler.h"
#include "../InputSettings.h"
#include "../../../libs/MathUtility.h"


PlayerInput::PlayerInput():MessageHandler(new WindowsApplicationMessageHandler())
{
}

bool PlayerInput::IsPressed(FKey InKey) const
{
	if (InKey == EKeys::AnyKey)
	{
		// Is there any key that is down
		for (const auto& [key, value] : KeyStateMap)
		{
			if (key.IsDigital() && value.bDown)
			{
				return true;
			}
		}
	}
	else if (KeyStateMap.contains(InKey))
	{
		return KeyStateMap.at(InKey).bDown;
	}

	return false;
}

bool PlayerInput::WasJustPressed(FKey InKey) const
{
	if (InKey == EKeys::AnyKey)
	{
		// Is there any key that has just been pressed
		for (const auto& [key, value] : KeyStateMap)
		{
			if (!key.IsAnalog() && value.EventCounts[IE_Pressed] > 0)
			{
				return true;
			}
		}
	}
	else if (KeyStateMap.contains(InKey))
	{
		return KeyStateMap.at(InKey).EventCounts[IE_Pressed] >0;
	}

	return false;
}

bool PlayerInput::WasJustReleased(FKey InKey) const
{
	if (InKey == EKeys::AnyKey)
	{
		// Is there any key that has just been pressed
		for (const auto& [key, value] : KeyStateMap)
		{
			if (!key.IsAnalog() && value.EventCounts[IE_Released] > 0)
			{
				return true;
			}
		}
	}
	else if (KeyStateMap.contains(InKey))
	{
		return KeyStateMap.at(InKey).EventCounts[IE_Released] > 0;
	}

	return false;
}

bool PlayerInput::IsActionPressed(const std::string& ActionName) const
{
	std::set<FKey> ActionKeys;
	input_settings->GetActionMappingByName(ActionName, ActionKeys);
	return std::ranges::any_of(ActionKeys, [this](FKey const& ActionKey) {return IsPressed(ActionKey); });
}

bool PlayerInput::WasActionJustPressed(const std::string& ActionName) const
{
	std::set<FKey> ActionKeys;
	input_settings->GetActionMappingByName(ActionName, ActionKeys);
	bool bIsAnyOfJustPressed = false;
	FKey JustPressedKey = EKeys::Invalid;

	bool bIsAllOthersNotPressed = false;

	for (auto it = ActionKeys.begin(); it != ActionKeys.end(); ++it) {
		if (WasJustPressed(*it))
		{
			JustPressedKey = *it;
			bIsAnyOfJustPressed = true;
			break;
		}
	}

	if(bIsAnyOfJustPressed)
	{
		ActionKeys.erase(JustPressedKey);
		bIsAllOthersNotPressed = std::ranges::all_of(ActionKeys, [this](FKey const& ActionKey) {return !IsPressed(ActionKey); });
	}

	return bIsAnyOfJustPressed && bIsAllOthersNotPressed;
}

bool PlayerInput::WasActionJustReleased(const std::string& ActionName) const
{
	std::set<FKey> ActionKeys;
	input_settings->GetActionMappingByName(ActionName, ActionKeys);
	bool bIsAnyOfJustReleased = false;
	FKey JustReleasedKey = EKeys::Invalid;

	bool bIsAllOthersNotPressed = false;

	for (auto it = ActionKeys.begin(); it != ActionKeys.end(); ++it) {
		if (WasJustReleased(*it))
		{
			JustReleasedKey = *it;
			bIsAnyOfJustReleased = true;
			break;
		}
	}

	if (bIsAnyOfJustReleased)
	{
		bIsAllOthersNotPressed = std::ranges::all_of(ActionKeys, [this](FKey const& ActionKey) {return !IsPressed(ActionKey); });
	}

	bIsAnyOfJustReleased = std::ranges::any_of(ActionKeys, [this](FKey const& ActionKey) {return WasJustReleased(ActionKey); });
	bIsAllOthersNotPressed = std::ranges::all_of(ActionKeys, [this](FKey const& ActionKey) {return !IsPressed(ActionKey); });;

	return bIsAnyOfJustReleased && bIsAllOthersNotPressed;
}

float PlayerInput::GetKeyValue(FKey InKey) const
{
	return KeyStateMap.contains(InKey) ? KeyStateMap.at(InKey).Value.x : 0.f;
}

float PlayerInput::GetAxisValue(std::string const& AxisName) const
{
	std::set<FInputAxisKeyMapping> AxisKeys;
	input_settings->GetAxisMappingByName(AxisName, AxisKeys);

	float AxisValue = 0.f;

	if (input_settings->GetAxisMappings().contains(AxisName))
	{
		auto& AxisMappings = input_settings->GetAxisMappings().at(AxisName);

		for (auto& AxisMapping : AxisMappings)
		{
			AxisValue += GetKeyValue(AxisMapping.Key) * AxisMapping.Scale;
		}
	}

	return AxisValue;
}

FVector PlayerInput::GetProcessedVectorKeyValue(FKey InKey) const
{
	return KeyStateMap.contains(InKey) ? KeyStateMap.at(InKey).Value : FVector();
}

void PlayerInput::SetInputSettings(InputSettings* InInputSettings)
{
	input_settings = InInputSettings;
}

std::map<FKey, FKeyState>& PlayerInput::GetKeyStates()
{
	return KeyStateMap;
}

void PlayerInput::SetKeyStateValue(FKey const& Key, float Value)
{
	auto KeyState = FKeyState();
	KeyState.Value.x = Value;
	KeyStateMap[Key] = KeyState;
}

bool PlayerInput::IsKeyHandledByAction(FKey Key) const
{
	auto const bIsHandled = input_settings && std::ranges::any_of(input_settings->GetActionMappings(),
		[&Key](const std::pair<const std::string, std::set<FKey>>& Value)
		{
			return Value.second.contains(Key) || Value.second.contains(EKeys::AnyKey);
		});

	return bIsHandled;
}

float PlayerInput::DetermineAxisValue(const std::string& AxisName, std::set<FKey>& KeysToConsume)
{
	float AxisValue = 0.f;

	if (input_settings->GetAxisMappings().contains(AxisName))
	{
		auto& AxisMappings = input_settings->GetAxisMappings().at(AxisName);

		std::set<FInputAxisKeyMapping> KeysToConsumeAndAxisKeysIntersection;

		for (auto& keys_to_consume : AxisMappings)
		{
			if (KeysToConsume.contains(keys_to_consume.Key))
				KeysToConsumeAndAxisKeysIntersection.insert(keys_to_consume);
		}

		for (auto& AxisMapping : KeysToConsumeAndAxisKeysIntersection)
		{
			AxisValue += GetKeyValue(AxisMapping.Key) * AxisMapping.Scale;
		}
	}

	return AxisValue;
}