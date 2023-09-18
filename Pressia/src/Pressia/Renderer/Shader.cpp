#include "pspch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Pressia/Platform/OpenGL/OpenGLShader.h"

#include <glad/glad.h>

namespace Pressia {

	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::None:
				PS_CORE_ASSERT(false, "Renderer API::None is not currently supported");
				return nullptr;
			case RendererAPI::OpenGL:
				return new OpenGLShader(vertexSrc, fragmentSrc);
		}

		PS_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}