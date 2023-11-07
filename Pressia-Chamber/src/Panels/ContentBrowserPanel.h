#pragma once
#include "Pressia/Renderer/Texture.h"

#include <filesystem>

namespace Pressia {

	class ContentBrowserPanel {
	public:
		ContentBrowserPanel();

		void OnImGuiRender();
	private:
		uint32_t SetIconType(std::filesystem::path fileExtension);
	private:
		float m_Padding = 16.0f;
		float m_ThumbnailSize = 128;

		std::filesystem::path m_CurrentDirectory;

		Ref<Texture2D> m_FolderIcon;
		Ref<Texture2D> m_TextFileIcon;
		Ref<Texture2D> m_ScriptFileIcon;
		Ref<Texture2D> m_FileIcon;
	};
}


