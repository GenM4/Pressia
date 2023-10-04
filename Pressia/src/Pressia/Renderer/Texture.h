#pragma once

#include <string>
#include "Pressia/Core/Core.h"

namespace Pressia {

	class Texture {
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool operator ==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture {
	public:
		const glm::vec2* GetTexCoords() const { return m_TexCoords; }

		static Ref<Texture2D> Create(uint32_t width, uint32_t height);
		static Ref<Texture2D> Create(const std::string& path);

		static Ref<Texture2D> CreateSubTexture(const std::string& path, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteCells = { 1.0f, 1.0f }, const glm::vec2& spacing = { 0.0f, 0.0f });	//Create Subtexture from uniform h/w sprite sheet

	private:
		glm::vec2 m_TexCoords[4] = {
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};
	};

}


