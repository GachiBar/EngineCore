#include "IniReaderWriter.h"
#include "../nameof.hpp"

IniReaderWriter::IniReaderWriter(const std::string& filename): file(filename)
{
    file.generate(ini);
    file.read(ini);
}