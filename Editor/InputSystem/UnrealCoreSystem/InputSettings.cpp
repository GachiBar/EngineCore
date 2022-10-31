#include "InputSettings.h"
#include <ranges>

#include "../ConfigReaderWriter.h"
#include "NameMapping/NamingStructs.h"
#include "NameMapping/ActionMapping.h"

InputSettings::InputSettings()
	: bCaptureMouseOnLaunch(true)
{
}

void InputSettings::RemoveInvalidKeys()
{
	//TODO Remove from config ini config
}

void InputSettings::AddActionMapping(std::string const& ActionMap, const FKey& KeyMapping, bool bForceRebuildKeymaps)
{
	if (ActionMappings.contains(ActionMap))
		ActionMappings[ActionMap].insert(KeyMapping);
	else
		ActionMappings[ActionMap] = { KeyMapping };

	if (bForceRebuildKeymaps)
	{
		ForceRebuildKeymaps();
	}
}

void InputSettings::SaveKeyMappings() const
{
	ConfigReaderWriter->SaveAxisInput(AxisMappings);
	ConfigReaderWriter->SaveActionsInput(ActionMappings);
}

void InputSettings::GetActionMappingByName(const std::string& InActionName, std::set<FKey>& OutMappings)
{
	if (!InActionName.empty())
	{
		OutMappings = ActionMappings[InActionName];
	}
}

void InputSettings::RemoveActionMapping(const std::string& ActionMap, const FKey& KeyMapping, const bool bForceRebuildKeymaps)
{
	if (!ActionMappings.contains(ActionMap))
		return;

	ActionMappings.at(ActionMap).erase(KeyMapping);

	if (bForceRebuildKeymaps)
	{
		ForceRebuildKeymaps();
	}
}

void InputSettings::AddAxisMapping(std::string const& AxisMap, const FInputAxisKeyMapping& KeyMapping,
	bool bForceRebuildKeymaps)
{
	if (AxisMappings.contains(AxisMap))
		AxisMappings[AxisMap].insert(KeyMapping);
	else
	{
		AxisMappings.clear();

		std::set<FInputAxisKeyMapping> Set;
		Set.insert(KeyMapping);
		AxisMappings[AxisMap] = Set;
	}

	if (bForceRebuildKeymaps)
	{
		ForceRebuildKeymaps();
	}
}

void InputSettings::GetAxisMappingByName(const std::string& InAxisName, std::set<FInputAxisKeyMapping>& OutMappings)
{
	if (!InAxisName.empty())
	{
		OutMappings = AxisMappings[InAxisName];
	}
}

void InputSettings::RemoveAxisMapping(std::string const& AxisMap, const FInputAxisKeyMapping& InKeyMapping, const bool bForceRebuildKeymaps)
{
	if (!AxisMappings.contains(AxisMap))
		return;

	AxisMappings.at(AxisMap).erase(InKeyMapping);

	if (bForceRebuildKeymaps)
	{
		ForceRebuildKeymaps();
	}
}

void InputSettings::GetActionNames(std::set<std::string>& OutActionNames) const
{
	OutActionNames.clear();
	auto kv = std::views::keys(ActionMappings);
	OutActionNames.insert(kv.begin(), kv.end());
}

void InputSettings::GetAxisNames(std::set<std::string>& OutAxisNames) const
{
	OutAxisNames.clear();
	auto kv = std::views::keys(AxisMappings);
	OutAxisNames.insert(kv.begin(), kv.end());
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

		bFoundUniqueName = !DoesActionExist(NewActionMappingName);
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

		bFoundUniqueName = !DoesAxisExist(NewAxisMappingName);
	} while (!bFoundUniqueName);

	return NewAxisMappingName;
}

void InputSettings::AddActionMapping(std::string const& NewMap, const FKey& NewKeyMapping)
{
	if (ActionMappings.contains(NewMap))
		ActionMappings[NewMap].insert(NewKeyMapping);

	std::set<FKey> Set;
	Set.insert(NewKeyMapping);
	ActionMappings[NewMap] = Set;
}

void InputSettings::AddAxisMapping(std::string const& NewMap, const FInputAxisKeyMapping& NewKeyMapping)
{
	if (AxisMappings.contains(NewMap))
		AxisMappings[NewMap].insert(NewKeyMapping);

	std::set<FInputAxisKeyMapping> Set;
	Set.insert(NewKeyMapping);
	AxisMappings[NewMap] = Set;
}

/** Ask for all the action mappings */
const std::map<std::string, std::set<FKey>>& InputSettings::GetActionMappings() const
{
	return ActionMappings;
}

/** Ask for all the axis mappings */
const std::map<std::string, std::set<FInputAxisKeyMapping>>& InputSettings::GetAxisMappings() const
{
	return AxisMappings;
}

/** Finds unique action name based on existing action names */
bool InputSettings::DoesActionExist(const std::string InActionName)
{
	return ActionMappings.contains(InActionName);
}

/** Finds unique axis name based on existing action names */
bool InputSettings::DoesAxisExist(const std::string InAxisName)
{
	return AxisMappings.contains(InAxisName);
}

void InputSettings::PopulateAxisConfigs()
{

}
