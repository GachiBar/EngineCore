#include "InputSettings.h"

#include "NameMapping/NamingStructs.h"

InputSettings::InputSettings()
	: bCaptureMouseOnLaunch(true)
{
}

void InputSettings::RemoveInvalidKeys()
{
	//TODO Remove from config ini config
}

void InputSettings::SaveKeyMappings()
{
	ActionMappings.Sort();
	AxisMappings.Sort();
	SaveConfig();
}

void InputSettings::AddActionMapping(const FInputActionKeyMapping& KeyMapping, const bool bForceRebuildKeymaps)
{
	const auto find_result = std::ranges::find(ActionMappings, KeyMapping);
	if(find_result!= ActionMappings.end())
	{
		ActionMappings.push_back(KeyMapping);
	}
	if (bForceRebuildKeymaps)
	{
		ForceRebuildKeymaps();
	}
}

void InputSettings::GetActionMappingByName(const std::string& InActionName, std::vector<FInputActionKeyMapping>& OutMappings) const
{
	if (!InActionName.empty())
	{
		OutMappings.clear();

		std::ranges::copy_if(ActionMappings, std::begin(OutMappings), [&InActionName](FInputActionKeyMapping const& Value) {return Value.ActionName == InActionName; });
	}
}

void InputSettings::RemoveActionMapping(const FInputActionKeyMapping& KeyMapping, const bool bForceRebuildKeymaps)
{
	std::ranges::remove_if(ActionMappings, [&KeyMapping](FInputActionKeyMapping const& Value) {return Value == KeyMapping; });

	if (bForceRebuildKeymaps)
	{
		ForceRebuildKeymaps();
	}
}

void InputSettings::AddAxisMapping(const FInputAxisKeyMapping& KeyMapping, const bool bForceRebuildKeymaps)
{
	const auto find_result = std::ranges::find(AxisMappings, KeyMapping);
	if (find_result != AxisMappings.end())
	{
		AxisMappings.push_back(KeyMapping);
	}

	if (bForceRebuildKeymaps)
	{
		ForceRebuildKeymaps();
	}
}

void InputSettings::GetAxisMappingByName(const std::string& InAxisName, std::vector<FInputAxisKeyMapping>& OutMappings) const
{
	if (!InAxisName.empty())
	{
		OutMappings.clear();

		std::ranges::copy_if(AxisMappings, std::begin(OutMappings), [&InAxisName](FInputAxisKeyMapping const& Value) {return Value.AxisName == InAxisName; });
	}
}

void InputSettings::RemoveAxisMapping(const FInputAxisKeyMapping& InKeyMapping, const bool bForceRebuildKeymaps)
{
	std::ranges::remove_if(AxisMappings, [&InKeyMapping](FInputAxisKeyMapping const& Value) {return Value == InKeyMapping; });

	if (bForceRebuildKeymaps)
	{
		ForceRebuildKeymaps();
	}
}

void InputSettings::GetActionNames(std::vector<std::string>& OutActionNames) const
{
	OutActionNames.clear();

	for (const FInputActionKeyMapping& ActionMapping : ActionMappings)
		OutActionNames.push_back(ActionMapping.ActionName);
}

void InputSettings::GetAxisNames(std::vector<std::string>& OutAxisNames) const
{
	OutAxisNames.clear();

	for (const FInputAxisKeyMapping& AxisMapping : AxisMappings)
		OutAxisNames.push_back(AxisMapping.AxisName);
}

void InputSettings::ForceRebuildKeymaps()
{
	/*
	for (TObjectIterator<UPlayerInput> It; It; ++It)
	{
		It->ForceRebuildingKeyMaps(true);
	}
	*/
}


std::string InputSettings::GetUniqueActionName(const std::string& BaseActionMappingName)
{
	static int NewMappingCount = 0;
	std::string NewActionMappingName;
	bool bFoundUniqueName = false;

	do
	{
		// Create a numbered name and check whether it's already been used
		NewActionMappingName = std::string(BaseActionMappingName, ++NewMappingCount);

		bFoundUniqueName = !(DoesActionExist(NewActionMappingName));
	} while (!bFoundUniqueName);

	return NewActionMappingName;
}

std::string InputSettings::GetUniqueAxisName(const std::string& BaseAxisMappingName)
{
	static int NewMappingCount = 0;
	std::string NewAxisMappingName;
	bool bFoundUniqueName = false;

	do
	{
		// Create a numbered name and check whether it's already been used
		NewAxisMappingName = BaseAxisMappingName + std::to_string(++NewMappingCount);
		auto find_result = std::ranges::find_if(AxisMappings, [&NewAxisMappingName](FInputAxisKeyMapping const& Value)
		{
			return Value.AxisName == NewAxisMappingName;
		});

		bFoundUniqueName = find_result == AxisMappings.end();
	} while (!bFoundUniqueName);

	return NewAxisMappingName;
}

void InputSettings::AddActionMapping(const FInputActionKeyMapping& NewMapping)
{
	ActionMappings.push_back(NewMapping);
}

void InputSettings::AddAxisMapping(const FInputAxisKeyMapping& NewMapping)
{
	AxisMappings.push_back(NewMapping);
}

/** Ask for all the action mappings */
const std::vector <FInputActionKeyMapping>& InputSettings::GetActionMappings() const
{
	return ActionMappings;
}

/** Ask for all the axis mappings */
const std::vector <FInputAxisKeyMapping>& InputSettings::GetAxisMappings() const
{
	return AxisMappings;
}

struct FMatchMappingByName
{
	FMatchMappingByName(const std::string& InName)
		: Name(InName)
	{
	}

	bool operator() (const FInputActionKeyMapping& ActionMapping) const
	{
		return ActionMapping.ActionName == Name;
	}

	bool operator() (const FInputAxisKeyMapping& AxisMapping) const
	{
		return AxisMapping.AxisName == Name;
	}

	std::string Name;
};

/** Finds unique action name based on existing action names */
bool InputSettings::DoesActionExist(const std::string InActionName)
{
	return std::ranges::find_if(ActionMappings, FMatchMappingByName(InActionName)) != ActionMappings.end();
}

/** Finds unique axis name based on existing action names */
bool InputSettings::DoesAxisExist(const std::string InAxisName)
{
	return std::ranges::find_if(AxisMappings, FMatchMappingByName(InAxisName)) != AxisMappings.end();
}

void InputSettings::PopulateAxisConfigs()
{

}
