#pragma once
#include <map>
#include <set>
#include <vector>
#include <string>
#include "KeyStruct.h"

#include "NameMapping/ActionMapping.h"

class ConfigReaderWriter;
using uint8 = unsigned char;

class InputSettings
{
public:
	InputSettings();
private:
	uint8 bAltEnterTogglesFullscreen:1;

	uint8 bF11TogglesFullscreen : 1;

	// Scale the mouse based on the player camera manager's field of view
	uint8 bEnableFOVScaling:1;

	/** Controls if the viewport will capture the mouse on Launch of the application */
	uint8 bCaptureMouseOnLaunch:1;

	// The scaling value to multiply the field of view by
	float FOVScale = 0.01111f;

private:
	/** Map of Action Mappings */
	std::map<std::string, std::set<FKey>> ActionMappings;

	/** Map of Axis Mappings */
	std::map<std::string, std::set<FInputAxisKeyMapping>> AxisMappings;

public:
	/** editor keys that work in the game */
	std::set<FKey> ExclusiveKeys;

	static void RemoveInvalidKeys(std::map<std::string, std::set<FInputAxisKeyMapping>>& InAxisMappings);

	static void RemoveInvalidKeys(std::map<std::string, std::set<FKey>>& InActionMappings);

	void GetActionMappingByName(const std::string& InActionName, std::set<FKey>& OutMappings);

	/** Programmatically remove an action mapping to the project defaults */
	void RemoveActionMapping(const std::string& ActionMap, const FKey& KeyMapping, bool bForceRebuildKeymaps = true);

	/** Retrieve all axis mappings by a certain name. */
	void GetAxisMappingByName(const std::string& InAxisName, std::set<FInputAxisKeyMapping>& OutMappings);

	/** Programmatically remove an axis mapping to the project defaults */
	void RemoveAxisMapping(std::string const& AxisMap, const FInputAxisKeyMapping& KeyMapping, bool bForceRebuildKeymaps = true);

	void AddDefaultEditorExclusiveKeys();
	bool IsKeyExclusive(FKey const & InKey) const;
	
	void RemoveAction(const std::string& InActionName);
	void RemoveAllActions();

	void RemoveAxis(const std::string& InAxisName);
	void RemoveAllAxis();

	void AddActionGroup(const std::string& InActionName);
	void AddAxisGroup(const std::string& InAxisName);

	void RenameActionGroup(const std::string& InActionName, const std::string& InNewName);
	void RenameAxisGroup(const std::string& InAxisName, const std::string& InNewName);

	/** Flush the current mapping values to the config file */
	void SaveKeyMappingsToFile() const;

	void LoadKeyMappingsFromConfig();

	/** Populate a list of all defined action names */
	void GetActionNames(std::set<std::string>& OutActionNames) const;

	/** Populate a list of all defined axis names */
	void GetAxisNames(std::set<std::string>& OutAxisNames) const;

	/** When changes are made to the default mappings, push those changes out to PlayerInput key maps */
	void ForceRebuildKeymaps();

	/** Finds unique action name based on existing action names */
	std::string GetUniqueActionName(const std::string& BaseActionMappingName);
	/** Finds unique axis name based on existing action names */
	std::string GetUniqueAxisName(const std::string& BaseAxisMappingName);

	/** Append new mapping to existing list */
	void AddActionMapping(std::string const& NewMap, const FKey& NewKeyMapping);
	/** Append new mapping to existing list */
	void AddAxisMapping(std::string const& NewMap, const FInputAxisKeyMapping& NewKeyMapping);

	/** Ask for all the action mappings */
	const std::map<std::string, std::set<FKey>>& GetActionMappings() const;
	/** Ask for all the axis mappings */
	const std::map<std::string, std::set<FInputAxisKeyMapping>>& GetAxisMappings() const;

	/** Finds unique action name based on existing action names */
	bool DoesActionExist(const std::string& InActionName);
	/** Finds unique axis name based on existing action names */
	bool DoesAxisExist(const std::string& InAxisName);

private:
	std::shared_ptr<ConfigReaderWriter> ConfigReaderWriter;

	void PopulateAxisConfigs();
};
