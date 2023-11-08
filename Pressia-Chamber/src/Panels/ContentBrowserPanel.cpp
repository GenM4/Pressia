#include "ContentBrowserPanel.h"
#include "pspch.h"

#include <imgui/imgui.h>

namespace Pressia {

	extern const std::filesystem::path g_AssetPath = "Assets";	//	TODO: Make relative to project path once projects implemented

	ContentBrowserPanel::ContentBrowserPanel() : m_CurrentDirectory(g_AssetPath) {
		m_FolderIcon = Texture2D::Create("Resources/Contentbrowser/icon_folder.png");
		m_TextFileIcon = Texture2D::Create("Resources/Contentbrowser/icon_textfile.png");
		m_ScriptFileIcon = Texture2D::Create("Resources/Contentbrowser/icon_script.png");
		m_FileIcon = Texture2D::Create("Resources/Contentbrowser/icon_file.png");
	}

	void Pressia::ContentBrowserPanel::OnImGuiRender() {
		ImGui::Begin("Content Browser Panel");

		if (m_CurrentDirectory != g_AssetPath) {
			if (ImGui::Button("<-")) {
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		float panelWidth = ImGui::GetContentRegionAvail().x;
		float cellSize = m_ThumbnailSize + m_Padding;
		int columnCount = (int)(panelWidth / cellSize);

		ImGui::Columns(columnCount, 0, false);
		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory)) {
			auto relativePath = std::filesystem::relative(directoryEntry.path(), g_AssetPath).string();

			const auto& filename = directoryEntry.path().filename();
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton(relativePath.c_str(), (ImTextureID)SetIconType(filename.extension()), { m_ThumbnailSize, m_ThumbnailSize }, { 0, 1 }, { 1, 0 });
			ImGui::PopStyleColor();

			//	Drag and drop scenes into viewport
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
				ImGui::SetDragDropPayload("ContentBrowser_Item", relativePath.c_str(), relativePath.size() + 1, ImGuiCond_Once);
				ImGui::EndDragDropSource();
			}

			//	File navigation
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				if (directoryEntry.is_directory()) {
					m_CurrentDirectory /= filename;
				}
			}
			ImGui::TextWrapped(filename.string().c_str());

			ImGui::NextColumn();
		}
		ImGui::Columns(1);

		//ImGui::SliderFloat("Thumbnail", &thumbnailSize, 16, 512);		//	TODO: Add to settings menu
		//ImGui::SliderFloat("Padding", &padding, 1, 64);				//	TODO: Add to settings menu
		//	TODO: Status bar

		ImGui::End();
	}

	uint32_t ContentBrowserPanel::SetIconType(std::filesystem::path fileExtension) {
		if (fileExtension == "") {
			return m_FolderIcon->GetRendererID();
		}
		else if (fileExtension == ".txt") {
			return m_TextFileIcon->GetRendererID();
		}
		else if (fileExtension == ".cs" && fileExtension == ".cpp") {
			return m_ScriptFileIcon->GetRendererID();
		}
		else {
			return m_FileIcon->GetRendererID();
		}
	}
}

