#pragma once

#include <map>
#include <set>

#include "../libs/ini/IniReaderWriter.h"
struct FKey;
struct FInputAxisKeyMapping;

class ConfigReaderWriter  final: public IniReaderWriter
{
public:
	ConfigReaderWriter(const std::string& filename);

	void SaveActionsInput(std::map<std::string, std::set<FKey>> const & ActionMappings);
	void SaveAxisInput(std::map<std::string, std::set<FInputAxisKeyMapping>> const & ActionMappings);
private:
	std::string InputAxisNameSection = "Input.AxisMappings";
	std::string InputActionNameSection = "Input.ActionMappings";
};

