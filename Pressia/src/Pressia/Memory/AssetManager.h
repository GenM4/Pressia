#pragma once

#include "Pressia/Memory/Asset.h"

#include <filesystem>

namespace Pressia {

	class AssetManager {
	public:
		AssetManager();
		~AssetManager();


	private:
		void ParseFilesystem();
	private:
		std::vector<std::filesystem::path> m_AssetPaths;
		std::filesystem::path m_AssetDirectoryPath = "Assets";	//	TODO: Make relative to project path once projects implemented


		friend class Asset;
	};
}
