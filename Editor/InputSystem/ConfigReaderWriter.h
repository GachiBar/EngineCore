#pragma once

#include <map>
#include <set>

#include "../libs/ini/IniReaderWriter.h"
#include "UnrealCoreSystem/KeyStruct.h"
#include "UnrealCoreSystem/NameMapping/ActionMapping.h"

struct FKey;

class ConfigReaderWriter  final: public IniReaderWriter
{
public:
	ConfigReaderWriter(const std::string& filename);

	void SaveActionsInput(std::map<std::string, std::set<FKey>> const & ActionMappings);
	void SaveAxisInput(std::map<std::string, std::set<FInputAxisKeyMapping>> const & AxisMappings);

	void GetActionsFromIni(std::map<std::string, std::set<FKey>>& ActionMappings_out);
	void GetAxisFromIni(std::map<std::string, std::set<FInputAxisKeyMapping>>& AxisMappings_out);
private:
	std::string InputAxisNameSection = "Input.AxisMappings";
	std::string InputActionNameSection = "Input.ActionMappings";
};

