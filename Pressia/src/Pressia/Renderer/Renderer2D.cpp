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
		PS_PROFILE_RENDERER_FUNCTION();

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
		PS_PROFILE_RENDERER_FUNCTION();

		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		PS_PROFILE_RENDERER_FUNCTION();

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene() {
		PS_PROFILE_RENDERER_FUNCTION();
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const glm::vec4& color) { 		// Draw quad with 2 coord position and flat color
		DrawQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const glm::vec4& color) {			// Draw quad with 3 coord position and flat color
		PS_PROFILE_RENDERER_FUNCTION();

		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->TextureShader->SetFloat("u_TilingFactor", 1.0f);
		s_Data->WhiteTexture->Bind(); // Bind white texture

		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, position) * glm::scale(transform, { size.x, size.y, 1.0f }) * glm::rotate(transform, rotation, glm::vec3(0.0f, 0.0f, 1.0f));

		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->VA->Bind();
		RenderCommand::DrawIndexed(s_Data->VA);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4 tintColor) {	// Draw quad with 2 coord position and provided texture
		DrawQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4 tintColor) {	// Draw quad with 3 coord position and provided texture
		PS_PROFILE_RENDERER_FUNCTION();

		s_Data->TextureShader->SetFloat4("u_Color", tintColor);
		s_Data->TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		texture->Bind();

		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, position) * glm::scale(transform, { size.x, size.y, 1.0f }) * glm::rotate(transform, rotation, glm::vec3(0.0f, 0.0f, 1.0f));

		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->VA->Bind();
		RenderCommand::DrawIndexed(s_Data->VA);
	}

}