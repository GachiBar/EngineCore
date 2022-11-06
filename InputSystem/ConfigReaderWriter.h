#pragma once

#include <map>
#include <set>

#include "IniReaderWriter.h"
#include "InputCoreSystem/KeyStruct.h"
#include "InputCoreSystem/NameMapping/ActionMapping.h"


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

