#include "pspch.h"
#include "Renderer2D.h"
#include "Pressia/Renderer/RendererConfig.h"

#include "Pressia/Renderer/RenderCommand.h"
#include "Pressia/Renderer/Shader.h"
#include "Pressia/Renderer/VertexArray.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Pressia {

	struct QuadVertex {
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;

		//	Editor only
		int EntityID;
	};

	struct Renderer2DData {
		static const uint32_t MaxQuads = PS_MAX_BATCH_DRAW;							// Max number of quads per draw call, 
		static const uint32_t MaxVertices = MaxQuads * 4;							// 4 Vertices per quad
		static const uint32_t MaxIndices = MaxQuads * 6;							// 6 Indices per quad
		static const uint32_t MaxTextureSlots = PS_MAX_TEXTURE_SLOTS;

		Ref<VertexArray> QuadVA;
		Ref<VertexBuffer> QuadVB;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;										// 0 = white texture

		glm::vec4 QuadVertexPositions[4] = { { -0.5f, -0.5f, 0.0f, 1.0f },
											 {  0.5f, -0.5f, 0.0f, 1.0f },
											 {  0.5f,  0.5f, 0.0f, 1.0f },
											 { -0.5f,  0.5f, 0.0f, 1.0f } };

		Renderer2D::Statistics Stats;
	};

	static Renderer2DData s_Data;


	void Renderer2D::Init() {
		PS_PROFILE_RENDERER_FUNCTION();

		s_Data.QuadVA = VertexArray::Create();


		s_Data.QuadVB = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.QuadVB->SetLayout({
			{ ShaderDataType::Float3,	"a_Position"		},
			{ ShaderDataType::Float4,	"a_Color"			},
			{ ShaderDataType::Float2,	"a_TexCoord"		},
			{ ShaderDataType::Float,	"a_TexIndex"		},
			{ ShaderDataType::Float,	"a_TilingFactor"	},
			{ ShaderDataType::Int,		"a_EntityID"		}
			});
		s_Data.QuadVA->AddVertexBuffer(s_Data.QuadVB);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6) {
			// Counter clockwise quad draw order
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		auto quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVA->SetIndexBuffer(quadIB);

		delete[] quadIndices;

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));

		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++) {
			samplers[i] = i;
		}

		s_Data.TextureShader = Shader::Create("Assets/Shaders/Texture.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;
	}

	void Renderer2D::Shutdown() {
		PS_PROFILE_RENDERER_FUNCTION();

	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& cameraTransform) {
		PS_PROFILE_RENDERER_FUNCTION();

		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(cameraTransform);

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", viewProj);

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::BeginScene(const EditorCamera& camera) {
		glm::mat4 viewProj = camera.GetViewProjection();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", viewProj);

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		PS_PROFILE_RENDERER_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene() {
		PS_PROFILE_RENDERER_FUNCTION();

		if (s_Data.QuadIndexCount == 0)
			return;

		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		s_Data.QuadVB->SetData(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush() {
		PS_PROFILE_RENDERER_FUNCTION();

		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++) {
			s_Data.TextureSlots[i]->Bind(i);
		}

		RenderCommand::DrawIndexed(s_Data.QuadVA, s_Data.QuadIndexCount);

		#if PS_RECORD_RENDERER_STATS
		s_Data.Stats.DrawCalls++;
		#endif
	}

	void Renderer2D::FlushAndReset() {
		EndScene();

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {	// Draw quad with 2 coord position and flat color
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {	// Draw quad with 3 coord position and flat color
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, position) * glm::scale(transform, { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const glm::vec4& color) {	// Draw quad with 2 coord position, rotation, and flat color
		DrawQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const glm::vec4& color) {	// Draw quad with 3 coord position, rotation, and flat color
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, position) * glm::scale(transform, { size.x, size.y, 1.0f }) * glm::rotate(transform, rotation, glm::vec3(0.0f, 0.0f, 1.0f));

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID) {	// Main draw function, takes calculated transform to draw
		PS_PROFILE_RENDERER_FUNCTION();

		constexpr float textureIndex = 0.0f;	//White texture slot
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = {
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};

		if (s_Data.QuadIndexCount >= s_Data.MaxIndices) {	//Check if max number of quads have been constructed
			FlushAndReset();
		}

		for (size_t i = 0; i < quadVertexCount; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		#if PS_RECORD_RENDERER_STATS
		s_Data.Stats.QuadCount++;
		#endif

		#if NON_BATCHED_IMPL
		s_Data.TextureShader->SetFloat4("u_Color", color);
		s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
		s_Data.WhiteTexture->Bind(); // Bind white texture

		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, position) * glm::scale(transform, { size.x, size.y, 1.0f }) * glm::rotate(transform, rotation, glm::vec3(0.0f, 0.0f, 1.0f));

		s_Data.TextureShader->SetMat4("u_Transform", transform);

		s_Data.QuadVA->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVA);
		#endif
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4 tintColor) {	// Draw quad with 2 coord position and provided texture
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4 tintColor) {	// Draw quad with 3 coord position and provided texture
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, position) * glm::scale(transform, { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4 tintColor) {	// Draw quad with 2 coord position, rotation, and provided texture
		DrawQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4 tintColor) {	// Draw quad with 3 coord position, rotation, and provided texture
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, position) * glm::scale(transform, { size.x, size.y, 1.0f }) * glm::rotate(transform, rotation, glm::vec3(0.0f, 0.0f, 1.0f));

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4 tintColor, int entityID) {	// Main draw function, takes calculated transform to draw
		PS_PROFILE_RENDERER_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		const glm::vec2* textureCoords = texture->GetTexCoords();

		if (s_Data.QuadIndexCount >= s_Data.MaxIndices) {	//Check if max number of quads have been constructed
			FlushAndReset();
		}

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {	//Look through current textures to find current texture (for repeated texture use)
			if (*s_Data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) {	//Add new texture
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		#if PS_RECORD_RENDERER_STATS
		s_Data.Stats.QuadCount++;
		#endif


		#if NON_BATCHED_IMPL
		s_Data.TextureShader->SetFloat4("u_Color", tintColor);
		s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		texture->Bind();

		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, position) * glm::scale(transform, { size.x, size.y, 1.0f });

		s_Data.TextureShader->SetMat4("u_Transform", transform);

		s_Data.QuadVA->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVA);
		#endif
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID) {
		DrawQuad(transform, src.Color, entityID);
	}

	Renderer2D::Statistics Renderer2D::GetStats() {
		return s_Data.Stats;
	}

	void Renderer2D::ResetStats() {
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

}