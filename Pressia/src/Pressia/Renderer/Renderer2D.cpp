#include "pspch.h"
#include "Renderer2D.h"

#include "Pressia/Renderer/RenderCommand.h"
#include "Pressia/Renderer/Shader.h"
#include "Pressia/Renderer/VertexArray.h"

#include "Platform/OpenGL/OpenGLShader.h" // Remove

namespace Pressia {

	struct Renderer2DStorage {
		Ref<VertexArray> VA;
		Ref<Shader> Shader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init() {
		s_Data = new Renderer2DStorage();

		s_Data->VA = VertexArray::Create();

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		auto squareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({ { ShaderDataType::Float3, "a_Position" } });
		s_Data->VA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1 ,2, 2, 3, 0 };
		auto squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		s_Data->VA->SetIndexBuffer(squareIB);

		s_Data->Shader = Shader::Create("Assets/Shaders/FlatColorShader.glsl");
	}

	void Renderer2D::Shutdown() {
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->Shader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->Shader)->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->Shader)->UploadUniformMat4("u_Transform", glm::mat4(1.0f));
	}

	void Renderer2D::EndScene() {

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->Shader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->Shader)->UploadUniformFloat4("u_Color", color);

		s_Data->VA->Bind();
		RenderCommand::DrawIndexed(s_Data->VA);
	}

}