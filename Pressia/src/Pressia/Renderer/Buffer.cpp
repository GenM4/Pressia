#include "pspch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Pressia {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
				PS_CORE_ASSERT(false, "Renderer API::None is not currently supported");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return new OpenGLVertexBuffer(vertices, size);
		}

		PS_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
				PS_CORE_ASSERT(false, "Renderer API::None is not currently supported");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return new OpenGLIndexBuffer(indices, size);
		}

		PS_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}