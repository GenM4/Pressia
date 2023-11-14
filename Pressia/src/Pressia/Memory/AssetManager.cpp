#include "pspch.h"
#include "AssetManager.h"

#define PS_TEXTURE_FILE_EXTENSION ".png"
#define PS_SHADER_FILE_EXTENSION ".glsl"


namespace Pressia {

	Pressia::AssetManager::AssetManager() {
		ParseFilesystem();
	}

	Pressia::AssetManager::~AssetManager() {
	}

	void Pressia::AssetManager::ParseFilesystem() {
		for (auto& file : std::filesystem::directory_iterator(m_AssetDirectoryPath)) {
			auto& fileExtension = file.path().extension();
			if (fileExtension == PS_TEXTURE_FILE_EXTENSION || fileExtension == PS_SHADER_FILE_EXTENSION) {
				m_AssetPaths.push_back(file.path());
			}
		}
	}
}