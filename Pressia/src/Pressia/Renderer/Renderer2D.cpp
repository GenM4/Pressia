#include "pspch.h"
#include "Renderer2D.h"

#include "Pressia/Renderer/RenderCommand.h"
#include "Pressia/Renderer/Shader.h"
#include "Pressia/Renderer/VertexArray.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Pressia {

	struct Renderer2DStorage {
		Ref<VertexArray> VA;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init() {
		s_Data = new Renderer2DStorage();

		s_Data->VA = VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		auto squareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});
		s_Data->VA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1 ,2, 2, 3, 0 };
		auto squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		s_Data->VA->SetIndexBuffer(squareIB);

		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));

		s_Data->TextureShader = Shader::Create("Assets/Shaders/Texture.glsl");
		s_Data->TextureShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown() {
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene() {

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const glm::vec4& color) {
		DrawQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const glm::vec4& color) {
		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->WhiteTexture->Bind(); // Bind white texture

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f }) *
			glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f));

		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->VA->Bind();
		RenderCommand::DrawIndexed(s_Data->VA);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const Ref<Texture2D>& texture) {
		DrawQuad({ position.x, position.y, 0.0f }, size, rotation, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const Ref<Texture2D>& texture) {
		s_Data->TextureShader->SetFloat4("u_Color", { 0.2f, 0.3f, 0.8f, 0.9f }); // glm::vec4(1.0f));
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f }) *
			glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f));

		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->VA->Bind();
		RenderCommand::DrawIndexed(s_Data->VA);
	}

}