#include "pspch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Pressia {

	Ref<Texture2D> Pressia::Texture2D::Create(const std::string& path) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
				PS_CORE_ASSERT(false, "Renderer API::None is not currently supported");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return std::make_shared<OpenGLTexture2D>(path);
		}

		PS_CORE_ASSERT(false, "Unknown RendererAPI");

		return nullptr;
	}
}

