#include "IniReaderWriter.h"
#include "../nameof.hpp"

IniReaderWriter::IniReaderWriter(const std::string& filename): file(filename+".ini")
{
}

void IniReaderWriter::SetFileName(const std::string& filename)
{
	file = filename + ".ini";
}

void IniReaderWriter::RemoveKey(const std::string& section, const std::string& name)
{
	file.read(ini);
	if(ini.has(section))
	{
		const bool was_success = ini[section].remove(name);
		if (was_success)
			file.write(ini);
	}
}

void IniReaderWriter::RemoveSection(const std::string& section)
{
	file.read(ini);
	const bool was_success = ini.remove(section);
	if(was_success)
		file.write(ini);
}

IniReaderWriter::const_iterator IniReaderWriter::End() const
{
	return ini.end();
}

void IniReaderWriter::Clear()
{
	ini.clear();
	file.write(ini);
}

IniReaderWriter::const_iterator IniReaderWriter::Begin()
{
	file.read(ini);
	return ini.begin();
}
