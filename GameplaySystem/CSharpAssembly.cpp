#include "pch.h"
#include "CSharpAssembly.h"

#include <cassert>
#include <fstream>
#include <iostream>

namespace engine {

MonoAssembly* CSharpAssembly::GetMonoAssembly() {
	return assembly_;
}

CSharpAssembly::CSharpAssembly(const std::string& assembly_path) {
	uint32_t file_size = 0;
	char* file_data = ReadBytes(assembly_path, &file_size);

	// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
	MonoImageOpenStatus status;
	MonoImage* image = mono_image_open_from_data_full(file_data, file_size, 1, &status, 0);

	assert(status == MONO_IMAGE_OK && mono_image_strerror(status)); // TODO: exceptions and resources;

	assembly_ = mono_assembly_load_from_full(image, assembly_path.c_str(), &status, 0);
	mono_image_close(image);

	// Don't forget to free the file data
	delete[] file_data;
}

CSharpAssembly::~CSharpAssembly() {
	//delete assembly_;
}

char* CSharpAssembly::ReadBytes(const std::string& filepath, uint32_t* out_size)
{
	std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

	if (!stream) {
		// Failed to open the file
		return nullptr;
	}

	std::streampos end = stream.tellg();
	stream.seekg(0, std::ios::beg);
	uint32_t size = end - stream.tellg();

	if (size == 0) {
		// File is empty
		return nullptr;
	}

	char* buffer = new char[size];
	stream.read((char*)buffer, size);
	stream.close();

	*out_size = size;
	return buffer;
}

} // namespace engine

