#include "Sandbox2D.h"


#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1920.0f / 1080.0f) {
}

void Sandbox2D::OnAttach() {
	m_Texture = Pressia::Texture2D::Create("Assets/Textures/Penguin.png");
}

void Sandbox2D::OnDetach() {

}

void Sandbox2D::OnUpdate(Pressia::Timestep ts) {
	//Update
	m_CameraController.OnUpdate(ts);

	// Render
	Pressia::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Pressia::RenderCommand::Clear();


	Pressia::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Pressia::Renderer2D::DrawQuad(m_Quad1Pos, { 0.8f, 0.8f }, m_Quad1Angle, m_SquareColor);
	Pressia::Renderer2D::DrawQuad(m_Quad2Pos, { 2.0f, 2.0f }, m_Quad2Angle, m_Texture);

	Pressia::Renderer2D::EndScene();

}

void Sandbox2D::OnImGuiRender() {
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::SliderFloat2("Quad 1 Position", glm::value_ptr(m_Quad1Pos), -5.0f, 5.0f);
	ImGui::SliderFloat2("Quad 2 Position", glm::value_ptr(m_Quad2Pos), -5.0f, 5.0f);
	ImGui::SliderAngle("Quad 1 Angle", &m_Quad1Angle);
	ImGui::SliderAngle("Quad 2 Angle", &m_Quad2Angle);
	ImGui::End();
}

void Sandbox2D::OnEvent(Pressia::Event& e) {
	m_CameraController.OnEvent(e);
}
