#include "ConfigReaderWriterFactory.h"

ConfigReaderWriter* ConfigReaderWriterFactory::Create()
{
	return new ConfigReaderWriter{ "config" };
}
