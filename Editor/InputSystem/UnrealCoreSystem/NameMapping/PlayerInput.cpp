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
			if (!key.IsAnalog() && value.EventCounts[IE_Pressed].size() > 0)
			{
				return true;
			}
		}
	}
	else if (KeyStateMap.contains(InKey))
	{
		return KeyStateMap.at(InKey).EventCounts[IE_Pressed].size()>0;
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
			if (!key.IsAnalog() && value.EventCounts[IE_Released].size() > 0)
			{
				return true;
			}
		}
	}
	else if (KeyStateMap.contains(InKey))
	{
		return KeyStateMap.at(InKey).EventCounts[IE_Released].size() > 0;
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


bool PlayerInput::InputKey(FKey Key, EInputEvent Event, float AmountDepressed)
{
	FInputKeyParams Params;
	Params.Key = Key;
	Params.Event = Event;
	Params.Delta = FVector((double)AmountDepressed, 0.0, 0.0);

	return InputKey(Params);
}

bool PlayerInput::InputKey(const FInputKeyParams& Params)
{
	// MouseX and MouseY should not be treated as analog if there are no samples, as they need their EventAccumulator to be incremented 
	// in the case of a IE_Pressed or IE_Released
	const bool bTreatAsAnalog =
		Params.Key.IsAnalog() &&
		((Params.Key != EKeys::MouseX && Params.Key != EKeys::MouseY) || Params.NumSamples > 0);

	auto FindOrAdd = [](std::map<FKey, FKeyState>& kv_map, const FKey& Key)->FKeyState&
	{
		if (kv_map.contains(Key))
			return kv_map.find(Key)->second;
		return kv_map[Key] = FKeyState();
	};

	if (bTreatAsAnalog)
	{
		auto TestEventEdges = [this, &Params](FKeyState& TestKeyState, float EdgeValue)
		{
			// look for event edges
			if (EdgeValue == 0.f && Params.Delta.Length() != 0.f)
			{
				TestKeyState.EventAccumulator[IE_Pressed].push_back(++EventCount);
			}
			else if (EdgeValue != 0.f && Params.Delta.Length() == 0.f)
			{
				TestKeyState.EventAccumulator[IE_Released].push_back(++EventCount);
			}
			else
			{
				TestKeyState.EventAccumulator[IE_Repeat].push_back(++EventCount);
			}
		};

		// first event associated with this key, add it to the map
		FKeyState& KeyState = FindOrAdd(KeyStateMap,Params.Key);

		TestEventEdges(KeyState, KeyState.Value.x);

		// accumulate deltas until processed next
		KeyState.RawValueAccumulator += Params.Delta;
		

		// Mirror the key press to any associated paired axis
		FKey PairedKey = Params.Key.GetPairedAxisKey();
		if (PairedKey.IsValid())
		{
			//UE_LOG(LogTemp, Warning, TEXT("Spotted axis %s which is paired to %s"), *Key.GetDisplayName().ToString(), Key.GetPairedAxisKey().IsValid() ? *Key.GetPairedAxisKey().GetDisplayName().ToString() : TEXT("NONE"));

			EPairedAxis PairedAxis = Params.Key.GetPairedAxis();
			
			FKeyState& PairedKeyState = FindOrAdd(KeyStateMap, PairedKey);

			// Update accumulator for the appropriate axis
			if (PairedAxis == EPairedAxis::X)
			{
				PairedKeyState.RawValueAccumulator.x = KeyState.RawValueAccumulator.x;
				PairedKeyState.PairSampledAxes |= 0b001;
			}
			else if (PairedAxis == EPairedAxis::Y)
			{
				PairedKeyState.RawValueAccumulator.y = KeyState.RawValueAccumulator.x;
				PairedKeyState.PairSampledAxes |= 0b010;
			}

			else
			{
				//checkf(false, TEXT("Key %s has paired axis key %s but no valid paired axis!"), *Params.Key.GetFName().ToString(), *Params.Key.GetPairedAxisKey().GetFName().ToString());
			}

			// TODO: Will trigger multiple times for the paired axis key. Not desirable.
			TestEventEdges(PairedKeyState, PairedKeyState.Value.Length());
		}

		return false;
	}
	// Non-analog key
	else
	{
		// first event associated with this key, add it to the map
		FKeyState& KeyState = FindOrAdd(KeyStateMap,Params.Key);

		switch (Params.Event)
		{
		case IE_Pressed:
		case IE_Repeat:
			KeyState.RawValueAccumulator.X = Params.Delta.X;
			KeyState.EventAccumulator[Params.Event].Add(++EventCount);
			if (KeyState.bDownPrevious == false)
			{
				// check for doubleclick
				// note, a tripleclick will currently count as a 2nd double click.
				const float WorldRealTimeSeconds = World->GetRealTimeSeconds();
				if ((WorldRealTimeSeconds - KeyState.LastUpDownTransitionTime) < GetDefault<UInputSettings>()->DoubleClickTime)
				{
					KeyState.EventAccumulator[IE_DoubleClick].Add(++EventCount);
				}

				// just went down
				KeyState.LastUpDownTransitionTime = WorldRealTimeSeconds;
			}
			break;
		case IE_Released:
			KeyState.RawValueAccumulator.X = 0.f;
			KeyState.EventAccumulator[IE_Released].Add(++EventCount);
			break;
		case IE_DoubleClick:
			KeyState.RawValueAccumulator.X = Params.Delta.X;
			KeyState.EventAccumulator[IE_Pressed].Add(++EventCount);
			KeyState.EventAccumulator[IE_DoubleClick].Add(++EventCount);
			break;
		}
		KeyState.SampleCountAccumulator++;

		if (Params.Event == IE_Pressed)
		{
			return IsKeyHandledByAction(Params.Key);
		}

		return true;
	}

	return false;
}


void PlayerInput::Proccess()
 {
	 for (auto& KeyState : KeyStateMap)
	 {
		 
	 }
 }
