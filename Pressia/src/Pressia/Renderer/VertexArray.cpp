#include "pspch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Pressia {

	Ref<VertexArray> VertexArray::Create() {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
				PS_CORE_ASSERT(false, "Renderer API::None is not currently supported");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return std::make_shared<OpenGLVertexArray>();
		}

		PS_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}