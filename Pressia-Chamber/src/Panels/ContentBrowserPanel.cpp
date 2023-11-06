#include "ContentBrowserPanel.h"
#include "pspch.h"

#include <imgui/imgui.h>

namespace Pressia {

	static std::filesystem::path s_AssetPath = "Assets";	//	TODO: Make relative to project path once projects implemented

	ContentBrowserPanel::ContentBrowserPanel() : m_CurrentDirectory(s_AssetPath) {

	}

	void Pressia::ContentBrowserPanel::OnImGuiRender() {
		ImGui::Begin("Content Browser Panel");

		if (m_CurrentDirectory != s_AssetPath) {
			if (ImGui::Button("<-")) {
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory)) {
			auto relativePath = std::filesystem::relative(directoryEntry.path(), s_AssetPath);

			const auto& path = directoryEntry.path();
			if (directoryEntry.is_directory()) {
				if (ImGui::Button(relativePath.filename().string().c_str())) {
					m_CurrentDirectory /= path.filename();
				}
			}
			else {
				if (ImGui::Button(relativePath.filename().string().c_str())) {

				}
			}

		}

		ImGui::End();
	}
}

