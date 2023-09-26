#include "Sandbox2D.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1920.0f / 1080.0f) {
}

void Sandbox2D::OnAttach() {
	m_SquareVA = Pressia::VertexArray::Create();

	float squareVertices[3 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	auto squareVB = Pressia::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
	squareVB->SetLayout({ { Pressia::ShaderDataType::Float3, "a_Position" } });
	m_SquareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1 ,2, 2, 3, 0 };
	auto squareIB = Pressia::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	m_SquareVA->SetIndexBuffer(squareIB);

	m_Shader = Pressia::Shader::Create("Assets/Shaders/FlatColorShader.glsl");
}

void Sandbox2D::OnDetach() {

}

void Sandbox2D::OnUpdate(Pressia::Timestep ts) {
	//Update
	m_CameraController.OnUpdate(ts);

	// Render
	Pressia::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Pressia::RenderCommand::Clear();


	Pressia::Renderer::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<Pressia::OpenGLShader>(m_Shader)->Bind();
	std::dynamic_pointer_cast<Pressia::OpenGLShader>(m_Shader)->UploadUniformFloat4("u_Color", m_SquareColor);

	Pressia::Renderer::Submit(m_Shader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(0.75f)));

	Pressia::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender() {
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Pressia::Event& e) {
	m_CameraController.OnEvent(e);
}
