#pragma once

#include <DirectXTex.h>

class TextureLoader {
public:
	static bool LoadWic(std::wstring path, DirectX::ScratchImage& image) {
		const HRESULT hr = LoadFromWICFile(
			path.c_str(),
			DirectX::WIC_FLAGS_NONE,
			nullptr,
			image);
		
		return SUCCEEDED(hr);
	}

	static bool LoadWicSrgb(std::wstring path, DirectX::ScratchImage& image) {
		const HRESULT hr = LoadFromWICFile(
			path.c_str(),
			DirectX::WIC_FLAGS_DEFAULT_SRGB,
			nullptr,
			image);

		return SUCCEEDED(hr);
	}
};