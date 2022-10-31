#include "InputSystem/ConfigReaderWriter.h"

ConfigReaderWriter::ConfigReaderWriter(const std::string& filename):IniReaderWriter(filename)
{
}

void ConfigReaderWriter::SaveActionsInput(std::map<std::string, std::set<FKey>> const& ActionMappings)
{
}

void ConfigReaderWriter::SaveAxisInput(std::map<std::string, std::set<FInputAxisKeyMapping>> const& ActionMappings)
{
}
