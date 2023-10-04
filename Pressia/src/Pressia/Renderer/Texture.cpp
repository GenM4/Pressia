#include "pspch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Pressia {
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
				PS_CORE_ASSERT(false, "Renderer API::None is not currently supported");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLTexture2D>(width, height);
		}

		PS_CORE_ASSERT(false, "Unknown RendererAPI");

		return nullptr;
	}

	Ref<Texture2D> Pressia::Texture2D::Create(const std::string& path) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
				PS_CORE_ASSERT(false, "Renderer API::None is not currently supported");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLTexture2D>(path);
		}

		PS_CORE_ASSERT(false, "Unknown RendererAPI");

		return nullptr;
	}

	Ref<Texture2D> Texture2D::CreateSubTexture(const std::string& path, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteCells, const glm::vec2& spacing) {
		Ref<Texture2D> texture = Create(path);

		float spacingX = spacing.x;
		float spacingY = spacing.y;

		if (coords.x == 0.0f)
			spacingX = 0.0f;
		else
			spacingX = spacing.x * coords.x;
		if (coords.y == 0.0f)
			spacingY == 0.0f;
		else
			spacingY = spacing.y * coords.y;

		glm::vec2 min = { (coords.x * cellSize.x + spacingX) / texture->GetWidth(), (coords.y * cellSize.y + spacingY) / texture->GetHeight() };
		glm::vec2 max = { ((coords.x + spriteCells.x) * cellSize.x + spacingX) / texture->GetWidth(), ((coords.y + spriteCells.y) * cellSize.y + spacingY) / texture->GetHeight() };

		texture->m_TexCoords[0] = { min.x, min.y };
		texture->m_TexCoords[1] = { max.x, min.y };
		texture->m_TexCoords[2] = { max.x, max.y };
		texture->m_TexCoords[3] = { min.x, max.y };

		return texture;
	}
}

