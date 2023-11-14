#pragma once

#include "Pressia/Core/CoreConfig.h"

#include "Pressia/Memory/Memory.h"

#include <filesystem>

namespace Pressia {

	class Asset {
	public:
		Asset();
		~Asset();

		std::filesystem::path GetPath() { return m_AssetPath; }
		uint32_t GetAssetID() { return m_AssetID; }
	private:
		Observe<Asset> m_AssetRef;
		uint32_t m_AssetID;
		std::filesystem::path m_AssetPath;
	};
}


