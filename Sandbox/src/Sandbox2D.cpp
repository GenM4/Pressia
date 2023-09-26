#include "Sandbox2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1920.0f / 1080.0f) {
}

void Sandbox2D::OnAttach() {

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

	Pressia::Renderer2D::DrawQuad({ 0.0f,0.0f }, { 1.0f,1.0f }, m_SquareColor);

	Pressia::Renderer2D::EndScene();

	//              Will be fixed, add these to shader class at some point
	//std::dynamic_pointer_cast<Pressia::OpenGLShader>(m_Shader)->Bind();
	//std::dynamic_pointer_cast<Pressia::OpenGLShader>(m_Shader)->UploadUniformFloat4("u_Color", m_SquareColor);
}

void Sandbox2D::OnImGuiRender() {
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Pressia::Event& e) {
	m_CameraController.OnEvent(e);
}
