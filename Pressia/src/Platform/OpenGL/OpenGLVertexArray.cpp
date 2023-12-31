#include "pspch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Pressia {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
		switch (type) {
			case Pressia::ShaderDataType::Float:	return GL_FLOAT;
			case Pressia::ShaderDataType::Float2:	return GL_FLOAT;
			case Pressia::ShaderDataType::Float3:	return GL_FLOAT;
			case Pressia::ShaderDataType::Float4:	return GL_FLOAT;
			case Pressia::ShaderDataType::Mat3:		return GL_FLOAT;
			case Pressia::ShaderDataType::Mat4:		return GL_FLOAT;
			case Pressia::ShaderDataType::Int:		return GL_INT;
			case Pressia::ShaderDataType::Int2:		return GL_INT;
			case Pressia::ShaderDataType::Int3:		return GL_INT;
			case Pressia::ShaderDataType::Int4:		return GL_INT;
			case Pressia::ShaderDataType::Bool:		return GL_BOOL;
		}

		PS_CORE_ASSERT(false, "Unknown ShaderDataType");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray() {
		glCreateVertexArrays(1, &m_RendererID);

	}

	OpenGLVertexArray::~OpenGLVertexArray() {
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const {
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::UnBind() const {
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {
		PS_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();
		uint32_t index = 0;
		for (const auto& element : layout) {
			GLenum baseType = ShaderDataTypeToOpenGLBaseType(element.Type);

			if (baseType == GL_INT) {
				glEnableVertexAttribArray(index);
				glVertexAttribIPointer(index, element.GetComponentCount(), baseType, layout.GetStride(), (const void*)element.Offset);
			}
			else {
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index, element.GetComponentCount(), baseType, element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.Offset);
			}

			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);

	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}