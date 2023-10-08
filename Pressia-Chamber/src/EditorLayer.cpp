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

		m_ActiveScene = CreateRef<Scene>();

		auto square = m_ActiveScene->CreateEntity("Square Entity");

		square.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.2f, 0.7f, 0.5f, 1.0f });

		m_SquareEntity = square;
	}

	void EditorLayer::OnDetach() {
		PS_PROFILE_FUNCTION();

	}

	void EditorLayer::OnUpdate(Timestep ts) {
		PS_PROFILE_FUNCTION();

		m_TPF = ts;

		//Update
		if (m_ViewportFocused)
			m_CameraController.OnUpdate(ts);

		// Render
		if (m_ResetRenderStats)
			Renderer2D::ResetStats();

		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();
		Renderer2D::BeginScene(m_CameraController.GetCamera());

		m_ActiveScene->OnUpdate(ts);	// Update Scene

		Renderer2D::EndScene();
		m_Framebuffer->Unbind();
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
		ImGui::Separator();
		if (m_SquareEntity) {
			ImGui::Text("%s", m_SquareEntity.GetComponent<TagComponent>().Tag.c_str());
			auto& squareColor = m_SquareEntity.GetComponent<SpriteRendererComponent>().Color;
			ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
		}
		ImGui::Separator();
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


		//	Scene Viewport
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

		if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize)) {
			m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
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