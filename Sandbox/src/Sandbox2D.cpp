#include "Sandbox2D.h"


#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <chrono>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1920.0f / 1080.0f) {
}

void Sandbox2D::OnAttach() {
	PS_PROFILE_FUNCTION();

	m_Texture = Pressia::Texture2D::Create("Assets/Textures/Penguin.png");
}

void Sandbox2D::OnDetach() {
	PS_PROFILE_FUNCTION();

}

void Sandbox2D::OnUpdate(Pressia::Timestep ts) {
	PS_PROFILE_FUNCTION();

	//Update
	m_CameraController.OnUpdate(ts);

	// Render
	{
		PS_PROFILE_SCOPE("Pre Rendering");
		Pressia::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Pressia::RenderCommand::Clear();
	}

	{
		PS_PROFILE_SCOPE("Rendering");
		Pressia::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Pressia::Renderer2D::DrawQuad({ m_Quad1Pos.x, m_Quad1Pos.y, -0.1f }, { 0.8f, 0.8f }, m_Quad1Angle, m_SquareColor);
		Pressia::Renderer2D::DrawQuad(m_Quad2Pos, { 2.0f, 2.0f }, m_Quad2Angle, m_Texture, m_TilingFactor, m_SquareColor);

		Pressia::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender() {
	PS_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::SliderFloat2("Quad 1 Position", glm::value_ptr(m_Quad1Pos), -5.0f, 5.0f);
	ImGui::SliderFloat2("Quad 2 Position", glm::value_ptr(m_Quad2Pos), -5.0f, 5.0f);
	ImGui::SliderAngle("Quad 1 Angle", &m_Quad1Angle);
	ImGui::SliderAngle("Quad 2 Angle", &m_Quad2Angle);

	ImGui::SliderFloat("Texture Tiling", &m_TilingFactor, 1.0f, 20.0f);

	ImGui::End();
}

void Sandbox2D::OnEvent(Pressia::Event& e) {
	m_CameraController.OnEvent(e);
}
