#include "EditorLayer.h"


#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <chrono>

namespace Pressia {

	EditorLayer::EditorLayer() : Layer("Sandbox2D"), m_CameraController(1920.0f / 1080.0f) {
	}

	void EditorLayer::OnAttach() {
		PS_PROFILE_FUNCTION();

		m_Texture = Texture2D::Create("Assets/Textures/Penguin.png");
		m_TextureMap = Texture2D::Create("Assets/Textures/colored_tilemap.png");
		m_Sprite = Texture2D::CreateSubTexture("Assets/Textures/colored_tilemap.png", { 12, 7 }, { 8, 8 }, { 1, 1 }, { 1, 1 });

		FramebufferSpecification fbspec;
		fbspec.Width = 1920;
		fbspec.Height = 1080;
		m_Framebuffer = Framebuffer::Create(fbspec);

		m_CameraController.SetZoomLevel(10.0f);
	}

	void EditorLayer::OnDetach() {
		PS_PROFILE_FUNCTION();

	}

	void EditorLayer::OnUpdate(Timestep ts) {
		PS_PROFILE_FUNCTION();

		m_TPF = ts;

		//Update
		m_CameraController.OnUpdate(ts);

		// Render
		{
			PS_PROFILE_SCOPE("Pre Rendering");
			m_Framebuffer->Bind();
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderCommand::Clear();
		}

		if (m_ResetRenderStats)
			Renderer2D::ResetStats();

		{
			PS_PROFILE_SCOPE("Rendering");
			Renderer2D::BeginScene(m_CameraController.GetCamera());

			Renderer2D::DrawQuad({ m_Quad1Pos.x, m_Quad1Pos.y + 5.0f, -0.1f }, { 0.8f, 0.8f }, m_Quad1Angle, m_SquareColor);
			Renderer2D::DrawQuad(m_Quad2Pos, { 2.0f, 2.0f }, m_Quad2Angle, m_Texture, m_TilingFactor, m_SquareColor);

			Renderer2D::DrawQuad({ -5.0f, 0.0f, 0.0f }, { m_QuadScale.x, m_QuadScale.y }, { 0.8f, 0.3f, 0.8f, 1.0f });
			Renderer2D::DrawQuad({ 0.0f, -5.0f, 0.0f }, { 2.0f, 2.0f }, m_Texture, m_TilingFactor);
			Renderer2D::DrawQuad({ 5.0f, 0.0f, 0.0f }, { 2.0f, 2.0f }, { 0.2f, 0.8f, 0.2f, 1.0f });
			Renderer2D::DrawQuad({ 10.0f, 0.0f, 0.0f }, { 2.0f, 2.0f }, m_Quad2Angle, m_Texture);

			Renderer2D::DrawQuad({ 0.0f, -10.0f, 0.0f }, { 2.0f, 2.0f }, m_TextureMap);
			Renderer2D::DrawQuad({ 5.0f, -10.0f, 0.0f }, { 2.0f, 2.0f }, m_Sprite);

			//Animated quad
			static float rotation = 0.0f;
			rotation += ts * 5.0f;
			Renderer2D::DrawQuad({ 15.0f, 0.0f, 0.0f }, { 2.0f, 2.0f }, rotation, m_Texture);

			Renderer2D::EndScene();

			Renderer2D::BeginScene(m_CameraController.GetCamera());

			for (float y = 0.0f; y < m_StressTestBound; y += 1.0f) {
				for (float x = 0.0f; x < m_StressTestBound; x += 1.0f) {
					Renderer2D::DrawQuad({ x, y, 0.0f }, { 1.0f, 1.0f }, 0.0f, { x / m_StressTestBound, y / m_StressTestBound, 0.3f, 1.0f });
				}
			}

			/*
			for (float y = -20.0f; y < 20.0f; y += 1.0f) {
				for (float x = -20.0f; x < 20.0f; x += 1.0f) {
					Renderer2D::DrawQuad({ x, y, 0.0f }, { 1.0f, 1.0f }, 0.0f, { x / 50.0f, y / 50.0f, 0.3f, 1.0f });
				}
			}
			*/
			Renderer2D::EndScene();
			m_Framebuffer->Unbind();
		}
	}

	void EditorLayer::OnImGuiRender() {
		PS_PROFILE_FUNCTION();

		static bool dockingEnabled = true;	// Enable/Disable docking (for debug)
		if (dockingEnabled)
			ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());	// Dockspace

		ImGui::Begin("Frame Rate");	//FPS Meter
		ImGui::Text("Frame Time: %5.3f ms", m_TPF * 1000.0f);
		ImGui::Text("Frame Rate: %.0f", 1 / m_TPF);
		ImGui::End();


		ImGui::Begin("Settings");
		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::SliderFloat2("Quad 1 Position", glm::value_ptr(m_Quad1Pos), -5.0f, 5.0f);
		ImGui::SliderFloat2("Quad 2 Position", glm::value_ptr(m_Quad2Pos), -5.0f, 5.0f);
		ImGui::SliderAngle("Quad 1 Angle", &m_Quad1Angle);
		ImGui::SliderAngle("Quad 2 Angle", &m_Quad2Angle);
		ImGui::SliderFloat2("Quad Scale", glm::value_ptr(m_QuadScale), 1.0f, 25.0f);
		ImGui::SliderFloat("Texture Tiling", &m_TilingFactor, 1.0f, 20.0f);
		ImGui::SliderFloat("Stress Test Bounds", &m_StressTestBound, 1.0f, 300.0f);
		ImGui::End();


		ImGui::Begin("Renderer Statistics");
		if (m_ResetRenderStats)
			ImGui::Text("Showing Renderer Statistics per frame");
		else
			ImGui::Text("Showing Total Renderer Statistics");
		ImGui::Checkbox("Reset every frame", &m_ResetRenderStats);
		auto stats = Renderer2D::GetStats();
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quad Count: %d", stats.QuadCount);
		ImGui::Text("Vertex Count: %d", stats.GetTotalVertexCount());
		ImGui::Text("Index Count: %d", stats.GetTotalIndexCount());
		ImGui::End();


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize)) {
			//m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			m_CameraController.ResizeBounds(viewportPanelSize.x, viewportPanelSize.y);
		}

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2{ 0,1 }, ImVec2{ 1,0 });
		ImGui::PopStyleVar();
		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e) {
		m_CameraController.OnEvent(e);
	}
}