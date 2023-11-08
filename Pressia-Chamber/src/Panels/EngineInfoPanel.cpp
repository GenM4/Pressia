#include "EngineInfoPanel.h"

#include "Pressia/Renderer/Renderer2D.h"

#include <imgui/imgui.h>

namespace Pressia {

	void EngineInfoPanel::OnUpdate(Timestep ts) {
		m_TimePerFrame = ts;
	}

	void EngineInfoPanel::OnImGuiRender() {
		ImGui::Begin("Engine Info Panel");
		ImGui::Text("Frame Counter");
		ImGui::Text("	Frame Time: %5.3f ms", m_TimePerFrame * 1000.0f);
		ImGui::Text("	Frame Rate: %.0f", 1 / m_TimePerFrame);

		ImGui::Separator();

		ImGui::Text("Renderer Statistics");
		if (m_ResetRenderStats)
			ImGui::Text("Showing Renderer Statistics per frame");
		else
			ImGui::Text("Showing Cumulative Renderer Statistics");
		ImGui::Checkbox("Reset every frame", &m_ResetRenderStats);
		auto stats = Renderer2D::GetStats();
		ImGui::Text("	Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("	Quad Count: %d", stats.QuadCount);
		ImGui::Text("	Vertex Count: %d", stats.GetTotalVertexCount());
		ImGui::Text("	Index Count: %d", stats.GetTotalIndexCount());

		ImGui::Separator();

		ImGui::End();
	}
}