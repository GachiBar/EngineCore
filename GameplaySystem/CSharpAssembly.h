#pragma once

#include <string>
#include <mono/metadata/assembly.h>

namespace engine {

class CSharpAssembly {
public:
	MonoAssembly& GetMonoAssembly();

	CSharpAssembly(const std::string& assembly_path);
	~CSharpAssembly();

private:
	MonoAssembly* assembly_;

	char* ReadBytes(const std::string& filepath, uint32_t* out_size);
};

} // namespace engine