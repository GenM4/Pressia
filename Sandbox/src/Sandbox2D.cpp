#include "Sandbox2D.h"


#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <chrono>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1920.0f / 1080.0f) {
}

void Sandbox2D::OnAttach() {
	PS_PROFILE_FUNCTION();

	m_Texture = Pressia::Texture2D::Create("Assets/Textures/Penguin.png");
	m_TextureMap = Pressia::Texture2D::Create("Assets/Textures/colored_tilemap.png");
	m_Sprite = Pressia::Texture2D::CreateSubTexture("Assets/Textures/colored_tilemap.png", { 12, 7 }, { 8, 8 }, { 1, 1 }, { 1, 1 });

	Pressia::FramebufferSpecification fbspec;
	fbspec.Width = 1920;
	fbspec.Height = 1080;
	m_Framebuffer = Pressia::Framebuffer::Create(fbspec);

	m_CameraController.SetZoomLevel(10.0f);
}

void Sandbox2D::OnDetach() {
	PS_PROFILE_FUNCTION();

}

void Sandbox2D::OnUpdate(Pressia::Timestep ts) {
	PS_PROFILE_FUNCTION();

	m_TPF = ts;

	//Update
	m_CameraController.OnUpdate(ts);

	// Render
	{
		PS_PROFILE_SCOPE("Pre Rendering");
		Pressia::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Pressia::RenderCommand::Clear();
	}

	if (m_ResetRenderStats)
		Pressia::Renderer2D::ResetStats();

	{
		PS_PROFILE_SCOPE("Rendering");
		Pressia::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Pressia::Renderer2D::DrawQuad({ m_Quad1Pos.x, m_Quad1Pos.y + 5.0f, -0.1f }, { 0.8f, 0.8f }, m_Quad1Angle, m_SquareColor);
		Pressia::Renderer2D::DrawQuad(m_Quad2Pos, { 2.0f, 2.0f }, m_Quad2Angle, m_Texture, m_TilingFactor, m_SquareColor);

		Pressia::Renderer2D::DrawQuad({ -5.0f, 0.0f, 0.0f }, { m_QuadScale.x, m_QuadScale.y }, { 0.8f, 0.3f, 0.8f, 1.0f });
		Pressia::Renderer2D::DrawQuad({ 0.0f, -5.0f, 0.0f }, { 2.0f, 2.0f }, m_Texture, m_TilingFactor);
		Pressia::Renderer2D::DrawQuad({ 5.0f, 0.0f, 0.0f }, { 2.0f, 2.0f }, { 0.2f, 0.8f, 0.2f, 1.0f });
		Pressia::Renderer2D::DrawQuad({ 10.0f, 0.0f, 0.0f }, { 2.0f, 2.0f }, m_Quad2Angle, m_Texture);

		Pressia::Renderer2D::DrawQuad({ 0.0f, -10.0f, 0.0f }, { 2.0f, 2.0f }, m_TextureMap);
		Pressia::Renderer2D::DrawQuad({ 5.0f, -10.0f, 0.0f }, { 2.0f, 2.0f }, m_Sprite);

		//Animated quad
		static float rotation = 0.0f;
		rotation += ts * 5.0f;
		Pressia::Renderer2D::DrawQuad({ 15.0f, 0.0f, 0.0f }, { 2.0f, 2.0f }, rotation, m_Texture);

		Pressia::Renderer2D::EndScene();

		Pressia::Renderer2D::BeginScene(m_CameraController.GetCamera());

		for (float y = 0.0f; y < m_StressTestBound; y += 1.0f) {
			for (float x = 0.0f; x < m_StressTestBound; x += 1.0f) {
				Pressia::Renderer2D::DrawQuad({ x, y, 0.0f }, { 1.0f, 1.0f }, 0.0f, { x / m_StressTestBound, y / m_StressTestBound, 0.3f, 1.0f });
			}
		}

		/*
		for (float y = -20.0f; y < 20.0f; y += 1.0f) {
			for (float x = -20.0f; x < 20.0f; x += 1.0f) {
				Pressia::Renderer2D::DrawQuad({ x, y, 0.0f }, { 1.0f, 1.0f }, 0.0f, { x / 50.0f, y / 50.0f, 0.3f, 1.0f });
			}
		}
		*/
		Pressia::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender() {
	PS_PROFILE_FUNCTION();

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
	auto stats = Pressia::Renderer2D::GetStats();
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quad Count: %d", stats.QuadCount);
	ImGui::Text("Vertex Count: %d", stats.GetTotalVertexCount());
	ImGui::Text("Index Count: %d", stats.GetTotalIndexCount());
	ImGui::End();

}

void Sandbox2D::OnEvent(Pressia::Event& e) {
	m_CameraController.OnEvent(e);
}
