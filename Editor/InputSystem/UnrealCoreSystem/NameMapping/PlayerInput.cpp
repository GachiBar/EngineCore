#include "PlayerInput.h"

#include <algorithm>
#include <iterator>

#include "ActionMapping.h"
#include "../InputSettings.h"
#include "../../../libs/MathUtility.h"


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

float PlayerInput::GetKeyValue(FKey InKey) const
{
	return KeyStateMap.contains(InKey) ? KeyStateMap.at(InKey).Value.x : 0.f;
}

FVector PlayerInput::GetProcessedVectorKeyValue(FKey InKey) const
{
	return KeyStateMap.contains(InKey) ? KeyStateMap.at(InKey).Value : FVector();
}

bool PlayerInput::IsKeyHandledByAction(FKey Key) const
{
	auto const bIsHandled = InputSettings && std::ranges::any_of(InputSettings->GetActionMappings(),
		[&Key](const std::pair<const std::string, std::set<FKey>>& Value)
		{
			return Value.second.contains(Key) || Value.second.contains(EKeys::AnyKey);
		});

	return bIsHandled;
}

float PlayerInput::DetermineAxisValue(const std::string& AxisName, std::set<FKey>& KeysToConsume)
{
	float AxisValue = 0.f;

	if (InputSettings->GetAxisMappings().contains(AxisName))
	{
		auto& AxisMappings = InputSettings->GetAxisMappings().at(AxisName);

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

void PlayerInput::Proccess()
 {
	 for (auto& KeyState : KeyStateMap)
	 {
		 
	 }
 }
