#pragma once
#include <map>

#include "../libs/ini/IniReaderWriter.h"

class ConfigReaderWriter  final: public IniReaderWriter
{
public:
	ConfigReaderWriter(const std::string& filename);
	//std::map<std::string,std::vector<>>
};

