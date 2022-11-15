#include "InputSettings.h"
#include <ranges>

#include "../ConfigReaderWriter.h"
#include "NameMapping/NamingStructs.h"
#include "../ConfigReaderWriterFactory.h"
#include "NameMapping/ActionMapping.h"

InputSettings::InputSettings()
	: 
	  bCaptureMouseOnLaunch(true)
{
	ConfigReaderWriterFactory ConfigFactory;
	ConfigReaderWriter.reset(ConfigFactory.Create());
}

void InputSettings::SaveKeyMappingsToFile() const
{
	ConfigReaderWriter->SaveAxisInput(AxisMappings);
	ConfigReaderWriter->SaveActionsInput(ActionMappings);
}

void InputSettings::LoadKeyMappingsFromConfig()
{
	ConfigReaderWriter->GetActionsFromIni(ActionMappings);
	ConfigReaderWriter->GetAxisFromIni(AxisMappings);
}

void InputSettings::RemoveInvalidKeys(std::map<std::string, std::set<FInputAxisKeyMapping>>& InAxisMappings)
{
	std::ranges::for_each(InAxisMappings, [](std::pair<const std::string, std::set<FInputAxisKeyMapping>>& AxisMapping)
		{
			std::erase_if(AxisMapping.second, [](FInputAxisKeyMapping const& KeyMapping)
				{
					return KeyMapping.Key.IsValid();
				});
		});
}

void InputSettings::RemoveInvalidKeys(std::map<std::string, std::set<FKey>>& InActionMappings)
{
	std::ranges::for_each(InActionMappings, [](std::pair<const std::string, std::set<FKey>>& ActionMapping)
		{
			std::erase_if(ActionMapping.second, [](FKey const& Key)
				{
					return Key.IsValid();
				});
		});
}

void InputSettings::GetActionMappingByName(const std::string& InActionName, std::set<FKey>& OutMappings)
{
	if (!InActionName.empty() && ActionMappings.contains(InActionName))
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

void InputSettings::GetAxisMappingByName(const std::string& InAxisName, std::set<FInputAxisKeyMapping>& OutMappings)
{
	if (!InAxisName.empty() && AxisMappings.contains(InAxisName))
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

void InputSettings::AddDefaultEditorExclusiveKeys()
{
	ExclusiveKeys.insert(EKeys::F11);
}

bool InputSettings::IsKeyExclusive(FKey const& InKey) const
{
	return ExclusiveKeys.contains(InKey);
}

void InputSettings::RemoveAction(const std::string& InActionName)
{
	ActionMappings.erase(InActionName);
}

void InputSettings::RemoveAllActions()
{
	ActionMappings.clear();
}

void InputSettings::RemoveAxis(const std::string& InAxisName)
{
	AxisMappings.erase(InAxisName);
}

void InputSettings::RemoveAllAxis()
{
	AxisMappings.clear();
}

void InputSettings::AddActionGroup(const std::string& InActionName)
{
	ActionMappings[InActionName];
}

void InputSettings::AddAxisGroup(const std::string& InAxisName)
{
	AxisMappings[InAxisName];
}

void InputSettings::RenameActionGroup(const std::string& InActionName, const std::string& InNewName)
{
	if(ActionMappings.contains(InActionName) && InActionName != InNewName)
	{
		ActionMappings[InActionName].swap(ActionMappings[InNewName]);
		ActionMappings.erase(InActionName);
	}
}

void InputSettings::RenameAxisGroup(const std::string& InAxisName, const std::string& InNewName)
{
	if (AxisMappings.contains(InAxisName) && InAxisName != InNewName)
	{
		AxisMappings[InAxisName].swap(AxisMappings[InNewName]);
		AxisMappings.erase(InAxisName);
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
	bool bFoundUniqueName;

	do
	{
		// Create a numbered name and check whether it's already been used
		NewActionMappingName = BaseActionMappingName + std::to_string(++NewMappingCount);

		bFoundUniqueName = !DoesActionExist(NewActionMappingName);
	} while (!bFoundUniqueName);

	return NewActionMappingName;
}

std::string InputSettings::GetUniqueAxisName(const std::string& BaseAxisMappingName)
{
	static int NewMappingCount = 0;
	std::string NewAxisMappingName;
	bool bFoundUniqueName;

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
	else
	{
		std::set<FKey> Set;
		Set.insert(NewKeyMapping);
		ActionMappings[NewMap] = Set;
	}
}

void InputSettings::AddAxisMapping(std::string const& NewMap, const FInputAxisKeyMapping& NewKeyMapping)
{
	if (AxisMappings.contains(NewMap))
		AxisMappings[NewMap].insert(NewKeyMapping);
	else
	{
		std::set<FInputAxisKeyMapping> Set;
		Set.insert(NewKeyMapping);
		AxisMappings[NewMap] = Set;
	}
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
bool InputSettings::DoesActionExist(const std::string& InActionName)
{
	return ActionMappings.contains(InActionName);
}

/** Finds unique axis name based on existing action names */
bool InputSettings::DoesAxisExist(const std::string& InAxisName)
{
	return AxisMappings.contains(InAxisName);
}

void InputSettings::PopulateAxisConfigs()
{

}
