#pragma once

#include "mono_config.h"
#include "mono_assembly.h"

namespace mono
{

bool init_with_mono_assembly_path(const std::string& mono_assembly_path, const std::string& domain, bool enable_debugging = false);
bool init_with_mono_dirs(const std::string& domain, bool enable_debugging = false);
void shutdown();
    
} // namespace mono
