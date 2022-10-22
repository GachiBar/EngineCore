#pragma once
#include "ConfigReaderWriter.h"

class ConfigReaderWriterFactory
{
public:
	ConfigReaderWriterFactory() = default;

	ConfigReaderWriter* Create();
};

