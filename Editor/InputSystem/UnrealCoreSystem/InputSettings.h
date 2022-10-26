#pragma once
#include <vector>
#include <string>
#include "KeyStruct.h"

class ConfigReaderWriter;
using uint8 = unsigned char;

class InputSettings
{
public:
	InputSettings();
private:
	uint8 bAltEnterTogglesFullscreen:1;

	uint8 bF11TogglesFullscreen : 1;

	// Mouse smoothing control
	uint8 bEnableMouseSmoothing:1;

	// Scale the mouse based on the player camera manager's field of view
	uint8 bEnableFOVScaling:1;

	/** Controls if the viewport will capture the mouse on Launch of the application */
	uint8 bCaptureMouseOnLaunch:1;

	// The scaling value to multiply the field of view by
	float FOVScale = 0.01111f;

	/** If a key is pressed twice in this amount of time it is considered a "double click" */
	float DoubleClickTime = 0.2f;

private:
	/** List of Action Mappings */
	std::vector<struct FInputActionKeyMapping> ActionMappings;

	/** List of Axis Mappings */
	std::vector<struct FInputAxisKeyMapping> AxisMappings;

public:
	/** The keys which open the console. */
	std::vector<FKey> ConsoleKeys;

	void RemoveInvalidKeys();

	/** Programmatically add an action mapping to the project defaults */
	void AddActionMapping(const FInputActionKeyMapping& KeyMapping, bool bForceRebuildKeymaps = true);

	void GetActionMappingByName(const std::string& InActionName, std::vector<FInputActionKeyMapping>& OutMappings) const;

	/** Programmatically remove an action mapping to the project defaults */
	void RemoveActionMapping(const FInputActionKeyMapping& KeyMapping, bool bForceRebuildKeymaps = true);

	/** Programmatically add an axis mapping to the project defaults */
	void AddAxisMapping(const FInputAxisKeyMapping& KeyMapping, bool bForceRebuildKeymaps = true);

	/** Retrieve all axis mappings by a certain name. */
	void GetAxisMappingByName(const std::string& InAxisName, std::vector<FInputAxisKeyMapping>& OutMappings) const;

	/** Programmatically remove an axis mapping to the project defaults */
	void RemoveAxisMapping(const FInputAxisKeyMapping& KeyMapping, bool bForceRebuildKeymaps = true);

	/** Flush the current mapping values to the config file */
	void SaveKeyMappings();

	/** Populate a list of all defined action names */
	void GetActionNames(std::vector<std::string>& OutActionNames) const;

	/** Populate a list of all defined axis names */
	void GetAxisNames(std::vector<std::string>& OutAxisNames) const;

	/** When changes are made to the default mappings, push those changes out to PlayerInput key maps */
	void ForceRebuildKeymaps();

	/** Finds unique action name based on existing action names */
	std::string GetUniqueActionName(const std::string& BaseActionMappingName);
	/** Finds unique axis name based on existing action names */
	std::string GetUniqueAxisName(const std::string& BaseAxisMappingName);

	/** Append new mapping to existing list */
	void AddActionMapping(const FInputActionKeyMapping& NewMapping);
	/** Append new mapping to existing list */
	void AddAxisMapping(const FInputAxisKeyMapping& NewMapping);

	/** Ask for all the action mappings */
	const std::vector<FInputActionKeyMapping>& GetActionMappings() const;
	/** Ask for all the axis mappings */
	const std::vector<FInputAxisKeyMapping>& GetAxisMappings() const;

	/** Finds unique action name based on existing action names */
	bool DoesActionExist(const std::string InActionName);
	/** Finds unique axis name based on existing action names */
	bool DoesAxisExist(const std::string InAxisName);

private:
	std::shared_ptr<ConfigReaderWriter> ConfigReaderWriter;

	void PopulateAxisConfigs();
};
