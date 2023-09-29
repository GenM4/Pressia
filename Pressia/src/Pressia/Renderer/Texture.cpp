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
}

