#include "InputSystem/ConfigReaderWriter.h"

ConfigReaderWriter::ConfigReaderWriter(const std::string& filename):IniReaderWriter(filename)
{
}

void ConfigReaderWriter::SaveActionsInput(std::map<std::string, std::set<FKey>> const& ActionMappings)
{
	for (auto& ActionMapping : ActionMappings)
	{
		SetValue(InputActionNameSection, ActionMapping.first, ActionMapping.second);
	}
}

void ConfigReaderWriter::SaveAxisInput(std::map<std::string, std::set<FInputAxisKeyMapping>> const& AxisMappings)
{
	for (auto& AxisMapping : AxisMappings)
	{
		SetValue(InputAxisNameSection, AxisMapping.first, AxisMapping.second);
	}
}

void ConfigReaderWriter::GetActionsFromIni(std::map<std::string, std::set<FKey>>& ActionMappings_out)
{
	ActionMappings_out.clear();
	file.read(ini);
	if(ini.has(InputActionNameSection))
	{
		auto SectionArray = ini[InputActionNameSection];
		for( const auto& key : SectionArray | std::views::keys)
		{
			auto Keys = GetValue<std::vector, FKey>(InputActionNameSection, key);
			ActionMappings_out[key] = std::set<FKey>(Keys.begin(), Keys.end());
		}
	}
}

void ConfigReaderWriter::GetAxisFromIni(std::map<std::string, std::set<FInputAxisKeyMapping>>& AxisMappings_out)
{
	AxisMappings_out.clear();
	file.read(ini);
	if (ini.has(InputAxisNameSection))
	{
		auto SectionArray = ini[InputAxisNameSection];
		for (const auto& key : SectionArray | std::views::keys)
		{
			auto Keys = GetValue<std::vector, FInputAxisKeyMapping>(InputAxisNameSection, key);
			AxisMappings_out[key] = std::set<FInputAxisKeyMapping>(Keys.begin(), Keys.end());
		}
	}
}
